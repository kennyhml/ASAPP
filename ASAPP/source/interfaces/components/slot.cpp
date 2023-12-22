#include <iostream>
#include "asapp/interfaces/components/slot.h"
#include <opencv2/highgui.hpp>
#include "asapp/items/items.h"

namespace asa::interfaces::components
{
    namespace
    {
        std::unordered_map<items::ItemData::ItemQuality, window::Color> color_per_quality{
            {asa::items::ItemData::PRIMITIVE, window::Color(130, 133, 135)},
            {asa::items::ItemData::RAMSHACKLE, window::Color(31, 166, 36)},
            {asa::items::ItemData::APPRENTICE, window::Color(49, 85, 176)},
            {asa::items::ItemData::JOURNEYMAN, window::Color(102, 51, 179)},
            {asa::items::ItemData::MASTERCRAFT, window::Color(164, 163, 28)},
            {asa::items::ItemData::ASCENDANT, window::Color(2, 167, 172)}
        };
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

    bool Slot::is_empty() const
    {
        static constexpr window::Color weight_text_color(128, 231, 255);

        const cv::Mat masked = get_mask(get_weight_area(), weight_text_color, 35);
        return countNonZero(masked) < 10;
    }

    bool Slot::has(items::Item& item, float* accuracy_out) const
    {
        cv::Mat source = window::screenshot(area);
        cv::Mat templ = item.get_inventory_icon();
        const cv::Mat mask = item.get_inventory_icon_mask();

        float confidence = 0.75f;
        switch (item.get_data().type) {
        case items::ItemData::CONSUMABLE:
        case items::ItemData::AMMO: confidence = 0.9f;
            break;

        case items::ItemData::EQUIPPABLE:
        {
            cv::cvtColor(source, source, cv::COLOR_RGB2GRAY);
            cv::cvtColor(templ, templ, cv::COLOR_RGB2GRAY);
            break;
        }
        }
        return window::locate_template(source, templ, confidence, mask, accuracy_out).
            has_value();
    }

    bool Slot::get_item(items::Item*& item_out, const bool verbose) const
    {
        const auto start = std::chrono::system_clock::now();

        const bool has_armor = has_armor_value();
        const bool has_damage = !has_armor && has_damage_value();
        const bool is_multi_stack = !(has_armor || has_damage) && is_stack();

        if (verbose) {
            std::cout << "[+] Attempting to recognize item, armor: " << has_armor <<
                ", dmg: " << has_damage << ", stack: " << is_multi_stack << "\n";
        }
        for (const std::vector<asa::items::Item**>& iter : items::iter_all()) {
            for (const auto& item : iter) {
                if (verbose) {
                    std::cout << "\r\t[-] Checking " << (*item)->get_name() << "... " <<
                        std::setw(20) << " " << std::flush;
                }

                const items::ItemData& item_data = (*item)->get_data();

                float acc = 0.f;
                if ((item_data.has_armor_value != has_armor) || (item_data.
                    has_damage_value != has_damage) || (item_data.stack_size == 1 &&
                    is_multi_stack) || !has(**item, &acc)) { continue; }

                const auto time_taken = std::chrono::duration_cast<
                    std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
                if (verbose) {
                    std::cout << std::format(
                        "\r\t[-] Determined '{}' within {} and {}% accuracy.",
                        (*item)->get_name(), time_taken,
                        acc * 100) << std::setw(20) << " " << std::endl;
                }

                item_out = new items::Item(*(*item), is_blueprint(item_data),
                                           get_quality());
                return true;
            }
        }
        if (verbose) { std::cerr << "\r\t[!] Failed to determine the item.\n"; }
        return false;
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

    bool Slot::is_stack() const
    {
        static constexpr window::Color col(128, 231, 255);

        const auto mask = window::get_mask(get_stack_size_area(), col, 20);
        return cv::countNonZero(mask) > 30;
    }

    bool Slot::is_blueprint(items::ItemData data) const
    {
        // if the item should have durability, the blueprint doesnt have
        // it meaning we can check for the durability bar to determine whether
        // we got a blueprint version or a crafted one.
        if (data.has_durability) {
            static constexpr window::Color col(1, 156, 136);
            const auto roi = get_spoil_or_durability_bar_area();
            const auto mask = window::get_mask(roi, col, 20);
            return cv::countNonZero(mask) < 30;
        }

        // blueprints always have 0.1 weight, so if the item has it then it should
        // be a blueprint
        return window::match_template(get_weight_area(), resources::text::bp_weight);
    }

    asa::items::ItemData::ItemQuality Slot::get_quality() const
    {
        const window::Rect quality_roi(area.x + 2, area.y + 60, 6, 6);

        for (const auto& [quality, color] : color_per_quality) {
            const auto mask = window::get_mask(quality_roi, color, 25);
            if (cv::countNonZero(mask) > 20) { return quality; }
        }

        return asa::items::ItemData::ItemQuality::PRIMITIVE;
    }
}
