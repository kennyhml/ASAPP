#include "asapp/interfaces/components/slot.h"
#include <opencv2/highgui.hpp>
#include "asapp/items/items.h"

namespace asa::interfaces::components
{
    namespace
    {
        std::unordered_map<items::ItemData::ItemQuality, window::Color> color_per_quality{
            {asa::items::ItemData::PRIMITIVE, window::Color{130, 133, 135}},
            {asa::items::ItemData::RAMSHACKLE, window::Color{31, 166, 36}},
            {asa::items::ItemData::APPRENTICE, window::Color{49, 85, 176}},
            {asa::items::ItemData::JOURNEYMAN, window::Color{102, 51, 179}},
            {asa::items::ItemData::MASTERCRAFT, window::Color{164, 163, 28}},
            {asa::items::ItemData::ASCENDANT, window::Color{2, 167, 172}}
        };

        constexpr int CACHED_LOC_PADDING = 2;
        std::unordered_map<std::string, window::Rect> cached_locs{};

        bool has_blueprint_variant(const items::ItemData::ItemType type)
        {
            switch (type) {
            case items::ItemData::ItemType::RESOURCE:
            case items::ItemData::ItemType::ARTIFACT:
            case items::ItemData::ItemType::CONSUMABLE: { return false; }
            default: { return true; }
            }
        }

        float get_confidence_for_category(const items::ItemData::ItemType category)
        {
            switch (category) {
            case items::ItemData::ARTIFACT:
            case items::ItemData::ATTACHMENT:
            case items::ItemData::AMMO: { return 0.87f; }
            case items::ItemData::WEAPON:
            case items::ItemData::EQUIPPABLE: { return 0.7f; }
            default: { return 0.8f; }
            }
        }

        float get_max_confidence_for_category(const items::ItemData::ItemType category)
        {
            switch (category) {
            case items::ItemData::ARTIFACT:
            case items::ItemData::ATTACHMENT:
            case items::ItemData::AMMO: { return 0.9f; }
            case items::ItemData::CONSUMABLE: { return 0.96f; }
            default: { return 0.8f; }
            }
        }

        bool is_grayscale_category(const items::ItemData::ItemType category)
        {
            return (category == items::ItemData::EQUIPPABLE || category ==
                items::ItemData::WEAPON);
        }
    }

    window::Rect Slot::get_stack_size_area() const
    {
        return {area.x + 3, area.y + 3, 30, 14};
    }

    window::Rect Slot::get_armor_or_damage_icon_area() const
    {
        return {area.x + 4, area.y + 4, 18, 18};
    }

    window::Rect Slot::get_spoil_or_durability_bar_area() const
    {
        return {area.x, area.y + 83, 87, 5};
    }

    window::Rect Slot::get_weight_area() const
    {
        return {area.x + 46, area.y + 69, 42, 14};
    }

    window::Rect Slot::get_hovered_area() const
    {
        return {area.x - 15, area.y - 15, area.width + 30, area.height + 30};
    }

    bool Slot::is_empty() const
    {
        static constexpr window::Color weight_text_color{128, 231, 255};
        const cv::Mat masked = get_mask(get_weight_area(), weight_text_color, 35);
        return countNonZero(masked) < 10;
    }

    bool Slot::is_hovered() const
    {
        static constexpr window::Color hovered_white{255, 255, 255};
        const auto roi = get_hovered_area();

        return cv::countNonZero(window::get_mask(roi, hovered_white, 20)) > 100;
    }

    bool Slot::has(items::Item& item, float* accuracy_out) const
    {
        const bool is_cached = cached_locs.contains(item.get_name());
        window::Rect roi = area;
        if (is_cached) {
            // adjust the region of interest according to our cached rect
            const auto cached_loc = cached_locs.at(item.get_name());
            roi.x += cached_loc.x;
            roi.y += cached_loc.y;
            roi.width = cached_loc.width;
            roi.height = cached_loc.height;
        }

        cv::Mat src = window::screenshot(roi);
        cv::Mat templ = item.get_inventory_icon();
        const cv::Mat mask = item.get_inventory_icon_mask();

        // Match options will differ based on item category.
        const auto category = item.get_data().type;
        const float conf = get_confidence_for_category(category);
        if (is_grayscale_category(category)) {
            cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
            cv::cvtColor(templ, templ, cv::COLOR_RGB2GRAY);
        }

        const auto match = window::locate_template(src, templ, conf, mask, accuracy_out);
        if (!match.has_value()) { return false; }

        if (!is_cached) {
            // create a cachec location allowing some variance
            const window::Rect cached_loc(match->x - CACHED_LOC_PADDING,
                                          match->y - CACHED_LOC_PADDING,
                                          match->width + (CACHED_LOC_PADDING * 2),
                                          match->height + (CACHED_LOC_PADDING * 2));
            cached_locs[item.get_name()] = cached_loc;
        }
        return true;
    }

    std::unique_ptr<items::Item> Slot::get_item() const
    {
        if (is_empty()) { return nullptr; }

        const auto start = std::chrono::system_clock::now();
        const PrederminationResult data = predetermine();
        bool perf_match_found = false;
        items::Item* best_match = nullptr;
        float best_match_accuracy = 0.f;

        for (const std::vector<asa::items::Item**>& iter : items::iter_all()) {
            if (perf_match_found) { break; }
            const float category_max_conf = get_max_confidence_for_category(
                (*iter[0])->get_data().type);

            for (const auto& item : iter) {
                float accuracy = 0.f;
                if (!data.matches((*item)->get_data()) || !has(**item, &accuracy)) {
                    continue;
                }
                if (accuracy > best_match_accuracy) {
                    best_match = *item;
                    best_match_accuracy = accuracy;
                    if (accuracy > category_max_conf) {
                        perf_match_found = true;
                        break;
                    }
                }
            }
        }

        if (!best_match) { return nullptr; }

        const auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start);

        return std::make_unique<items::Item>(*best_match,
                                             is_blueprint(best_match->get_data()),
                                             get_quality());
    }

    bool Slot::has_armor_value() const
    {
        return window::match_template(get_armor_or_damage_icon_area(),
                                      asa::resources::interfaces::armor, 0.8f);
    }

    bool Slot::has_damage_value() const
    {
        return window::match_template(get_armor_or_damage_icon_area(),
                                      asa::resources::interfaces::damage, 0.8f);
    }

    bool Slot::has_spoil_timer() const
    {
        const auto bar = get_spoil_or_durability_bar_area();
        static constexpr window::Color spoil_color{0, 214, 161};
        static constexpr window::Color spoiled_color{28, 110, 73};

        const cv::Mat left = window::get_mask(bar, spoil_color, 20);
        const cv::Mat spoiled = window::get_mask(bar, spoiled_color, 20);
        return cv::countNonZero(left | spoiled) > 100;
    }

    bool Slot::has_durability() const
    {
        const auto bar = get_spoil_or_durability_bar_area();
        static constexpr window::Color dura_color{1, 156, 136};
        static constexpr window::Color dura_lost_color{6, 25, 38};

        const cv::Mat left = window::get_mask(bar, dura_color, 20);
        const cv::Mat lost = window::get_mask(bar, dura_lost_color, 20);
        return cv::countNonZero(left | lost) > 100;
    }

    bool Slot::is_stack() const
    {
        static constexpr window::Color stack_count_color{128, 231, 255};
        const auto bar = get_stack_size_area();
        const auto mask = window::get_mask(bar, stack_count_color, 20);
        return cv::countNonZero(mask) > 30;
    }

    bool Slot::is_blueprint(const items::ItemData& data) const
    {
        // Consumables, artifacts or resources do not have blueprint variants.
        if (!has_blueprint_variant(data.type)) { return false; }

        // Blueprints do not have the durability bar that the crafted variant has.
        if (data.has_durability) { return !has_durability(); }

        // Blueprints always have 0.1 weight
        return window::match_template(get_weight_area(), resources::text::bp_weight,
                                      0.9f);
    }

    items::ItemData::ItemQuality Slot::get_quality() const
    {
        const window::Rect quality_roi(area.x + 2, area.y + 60, 6, 6);

        for (const auto& [quality, color] : color_per_quality) {
            if (cv::countNonZero(window::get_mask(quality_roi, color, 25)) > 20) {
                return quality;
            }
        }

        return items::ItemData::ItemQuality::NONE;
    }

    Slot::PrederminationResult Slot::predetermine() const
    {
        PrederminationResult res;

        // Only one of these can be true for any item
        if (has_armor_value()) { res.has_armor_modifier = true; }
        else if (has_damage_value()) { res.has_damage_modifier = true; }
        else if (is_stack()) { res.is_stack = true; }

        if (has_spoil_timer()) { res.has_spoil_bar = true; }
        else if (has_durability()) { res.has_durability_bar = true; }

        return res;
    }

    bool Slot::PrederminationResult::matches(const items::ItemData& data) const
    {
        // check modifiers
        if (data.has_armor_value != this->has_armor_modifier || data.has_damage_value !=
            this->has_damage_modifier) { return false; }

        if ((data.stack_size == 1) && this->is_stack) { return false; }

        if (data.has_spoil_timer != this->has_spoil_bar || ((data.has_durability != this->
            has_durability_bar) && !has_blueprint_variant(data.type))) { return false; }
        return true;
    }

    std::ostream& operator<<(std::ostream& os, const Slot::PrederminationResult& d)
    {
        return os << std::format(
            "Armor: {}, damage: {}, stack: {}, spoils: {}, durability: {})",
            d.has_armor_modifier, d.has_damage_modifier, d.is_stack, d.has_spoil_bar,
            d.has_durability_bar);
    }
}
