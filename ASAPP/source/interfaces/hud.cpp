#include "hud.h"
#include "../util/util.h"
#include "asapp/game/resources.h"
#include "asapp/game/window.h"

namespace asa::interfaces
{
    namespace
    {
        bool is_blinking(window::Rect icon, window::Color color,
                         std::chrono::milliseconds maxDuration =
                             std::chrono::milliseconds(500))
        {
            auto start = std::chrono::system_clock::now();
            while (!util::timedout(start, maxDuration)) {
                auto masked = window::get_mask(icon, color, 30);
                if (cv::countNonZero(masked) > 20) { return true; }
            }
            return false;
        }
    }

    bool HUD::item_removed(const window::Rect& area)
    {
        return window::match_template(area, resources::text::removed);
    }

    bool HUD::item_added(const window::Rect& area)
    {
        return window::match_template(area, resources::text::added);
    }

    bool HUD::is_player_overweight()
    {
        return is_blinking(weight_icon, blink_red_state_weight,
                           std::chrono::milliseconds(700));
    }

    bool HUD::is_player_broken_bones()
    {
        return is_blinking(health_icon, blink_red_state);
    }

    bool HUD::is_player_out_of_water()
    {
        return is_blinking(water_icon, blink_red_state);
    }

    bool HUD::is_player_out_of_food() { return is_blinking(food_icon, blink_red_state); }

    bool HUD::is_player_sprinting() { return false; }

    bool HUD::can_default_teleport()
    {
        return window::match_template(default_teleport, resources::text::default_teleport,
                                      0.5);
    }

    bool HUD::can_fast_travel()
    {
        return window::match_template(window::screenshot(), resources::text::fast_travel);
    }

    bool HUD::can_access_inventory()
    {
        return window::match_template(window::screenshot(),
                                      resources::text::access_inventory);
    }

    bool HUD::extended_information_is_toggled()
    {
        static window::Rect roi{14, 34, 134, 35};
        return window::match_template(roi, resources::text::day);
    }

    bool HUD::item_added(items::Item* item, window::Rect* roi_out)
    {
        auto roi = item ? item_icon_removed_or_added_area : item_added_area;
        if (!item) { return item_added(roi); }
        auto locations = window::locate_all_template(roi, item->get_notification_icon(),
                                                     0.75, item->
                                                     get_notification_icon_mask());

        for (const auto& rect : locations) {
            window::Rect match_roi(roi.x + rect.x + 20, roi.y + rect.y, 120, 25);
            if (item_added(match_roi)) {
                if (roi_out) { *roi_out = match_roi; }
                return true;
            }
        }
        return false;
    }

    bool HUD::item_removed(items::Item* item, window::Rect* roi_out)
    {
        auto roi = item ? item_icon_removed_or_added_area : item_removed_area;
        if (!item) { return item_removed(roi); }
        auto locations = window::locate_all_template(roi, item->get_notification_icon(),
                                                     0.75, item->
                                                     get_notification_icon_mask());

        for (const auto& rect : locations) {
            window::Rect match_roi(roi.x + rect.x + 20, roi.y + rect.y, 120, 25);
            if (item_removed(match_roi)) {
                if (roi_out) { *roi_out = match_roi; }
                return true;
            }
        }
        return false;
    }

    bool HUD::count_items_added(items::Item& item, int& amount_out)
    {
        window::Rect roi{0, 0, 0, 0};
        if (!item_added(&item, &roi)) { return false; }

        roi = {roi.x + 85, roi.y, 100, roi.height};

        auto x_loc = window::locate_template(roi, resources::text::x);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location." << std::endl;
            return false;
        }

        roi = {roi.x, roi.y, x_loc->x, roi.height};
        cv::Mat mask = window::get_mask(roi, window::Color(255, 255, 255), 50);
        window::set_tesseract_image(mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
        window::tessEngine->SetVariable("tessedit_char_whitelist", "0123456789");

        std::string result_string = window::tessEngine->GetUTF8Text();
        if (result_string.empty() || result_string == "\\n") {
            std::cerr << "[!] OCR failed, no result determined." << std::endl;
            return false;
        }

        amount_out = std::stoi(result_string);
        return true;
    }

    bool HUD::count_items_removed(items::Item& item, int& amount_out)
    {
        window::Rect roi{0, 0, 0, 0};
        if (!item_removed(&item, &roi)) { return false; }

        roi = {roi.x + 110, roi.y, 100, roi.height};

        auto x_loc = window::locate_template(roi, resources::text::x);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location." << std::endl;
            return false;
        }

        roi = {roi.x, roi.y, x_loc->x, roi.height};
        cv::Mat mask = window::get_mask(roi, window::Color(255, 255, 255), 50);
        window::set_tesseract_image(mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
        window::tessEngine->SetVariable("tessedit_char_whitelist", "0123456789");

        std::string result_string = window::tessEngine->GetUTF8Text();
        if (result_string.empty() || result_string == "\\n") {
            std::cerr << "[!] OCR failed, no result determined." << std::endl;
            return false;
        }

        amount_out = std::stoi(result_string);
        return true;
    }

    float HUD::get_water_amount() const
    {
        static window::Rect roi(1885, 806, 1, 43);
        static window::Color full(111, 150, 155);

        cv::Mat masked = window::get_mask(roi, full, 30);

        int amount_filled = cv::countNonZero(masked);
        return (static_cast<float>(amount_filled) / static_cast<float>(roi.height));
    }

    float HUD::get_food_amount() const { return 0; }

    float HUD::get_weight_amount() const { return 0; }
}
