#include "asapp/interfaces/hud.h"
#include "asapp/util/util.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include "asapp/game/resources.h"
#include "asapp/game/window.h"

namespace asa::interfaces
{
    namespace
    {
        bool is_blinking(const window::Rect& icon, const window::Color& color,
                         const std::chrono::milliseconds timeout =
                             std::chrono::milliseconds(500))
        {
            const auto start = std::chrono::system_clock::now();
            while (!util::timedout(start, timeout)) {
                if (cv::countNonZero(window::get_mask(icon, color, 30)) > 20) {
                    return true;
                }
            }
            return false;
        }
    }

    bool HUD::is_player_overweight() const
    {
        return is_blinking(weight_icon_, blink_red_state_weight_,
                           std::chrono::milliseconds(700));
    }

    bool HUD::is_player_broken_bones() const
    {
        return is_blinking(health_icon_, blink_red_state_);
    }

    bool HUD::is_player_out_of_water() const
    {
        return is_blinking(water_icon_, blink_red_state_);
    }

    bool HUD::is_player_out_of_food() const
    {
        return is_blinking(food_icon_, blink_red_state_);
    }

    bool HUD::is_player_sprinting() const { return false; }

    bool HUD::can_default_teleport() const
    {
        return window::match_template(default_teleport_,
                                      resources::text::default_teleport, 0.5);
    }

    bool HUD::can_fast_travel() const
    {
        return window::match_template(window::screenshot(), resources::text::fast_travel);
    }

    bool HUD::can_access_inventory() const
    {
        return window::match_template(window::screenshot(),
                                      resources::text::access_inventory);
    }

    bool HUD::extended_information_is_toggled() const
    {
        static window::Rect roi{14, 34, 134, 35};
        return window::match_template(roi, resources::text::day);
    }

    bool HUD::item_added(items::Item& item, window::Rect* roi_out) const
    {
        const window::Rect roi = item_icon_removed_or_added_area;
        const std::vector<window::Rect> locations = window::locate_all_template(
            roi, item.get_notification_icon(), 0.75f, item.get_notification_icon_mask());

        return std::ranges::any_of(locations.begin(), locations.end(),
                                   [roi](const window::Rect& rect) -> bool {
                                       return item_added({
                                           roi.x + rect.x + 20, roi.y + rect.y - 10, 120,
                                           25
                                       });
                                   });
    }

    bool HUD::item_removed(items::Item& item, window::Rect* roi_out) const
    {
        const window::Rect roi = item_icon_removed_or_added_area;
        const std::vector<window::Rect> locations = window::locate_all_template(
            roi, item.get_notification_icon(), 0.75f, item.get_notification_icon_mask());

        return std::ranges::any_of(locations.begin(), locations.end(),
                                   [roi](const window::Rect& rect) -> bool {
                                       return item_removed({
                                           roi.x + rect.x + 20, roi.y + rect.y - 10, 120,
                                           30
                                       });
                                   });
    }

    bool HUD::count_items_added(items::Item& item, int& amount_out) const
    {
        window::Rect roi{0, 0, 0, 0};
        if (!item_added(item, &roi)) { return false; }

        roi = {roi.x + 85, roi.y, 100, roi.height};

        auto x_loc = window::locate_template(roi, resources::text::x);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location.\n";
            return false;
        }

        roi = {roi.x, roi.y, x_loc->x, roi.height};
        const cv::Mat mask = window::get_mask(roi, window::Color{255, 255, 255}, 50);
        window::set_tesseract_image(mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
        window::tessEngine->SetVariable("tessedit_char_whitelist", "0123456789");

        const std::string result_string = window::tessEngine->GetUTF8Text();
        if (result_string.empty() || result_string == "\\n") {
            std::cerr << "[!] OCR failed, no result determined.\n";
            return false;
        }

        amount_out = std::stoi(result_string);
        return true;
    }

    bool HUD::count_items_removed(items::Item& item, int& amount_out) const
    {
        window::Rect roi{0, 0, 0, 0};
        if (!item_removed(item, &roi)) { return false; }

        roi = {roi.x + 110, roi.y, 100, roi.height};

        auto x_loc = window::locate_template(roi, resources::text::x);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location.\n";
            return false;
        }

        roi = {roi.x, roi.y, x_loc->x, roi.height};
        cv::Mat mask = window::get_mask(roi, window::Color{255, 255, 255}, 50);
        window::set_tesseract_image(mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
        window::tessEngine->SetVariable("tessedit_char_whitelist", "0123456789");

        const std::string result_string = window::tessEngine->GetUTF8Text();
        if (result_string.empty() || result_string == "\\n") {
            std::cerr << "[!] OCR failed, no result determined.\n";
            return false;
        }

        amount_out = std::stoi(result_string);
        return true;
    }

    float HUD::get_water_amount() const
    {
        static window::Rect roi(1885, 806, 1, 43);
        static window::Color full{111, 150, 155};

        const int amount_filled = cv::countNonZero(window::get_mask(roi, full, 30));
        return (static_cast<float>(amount_filled) / static_cast<float>(roi.height));
    }

    float HUD::get_food_amount() const { return 0; }

    float HUD::get_weight_amount() const { return 0; }

    bool HUD::item_removed(const window::Rect& area) const
    {
        return window::match_template(area, resources::text::removed);
    }

    bool HUD::item_added(const window::Rect& area) const
    {
        return window::match_template(area, resources::text::added);
    }
}
