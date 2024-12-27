#include "asa/ui/hud.h"
#include "asa/utility.h"
#include "asa/game/game.h"

#include <iostream>

#include "asa/core/state.h"

namespace asa
{
    namespace
    {
        bool is_blinking(const cv::Rect& icon, const cv::Vec3b& color,
                         const int min_matches = 500,
                         const std::chrono::milliseconds timeout = 500ms)
        {
            const auto start = std::chrono::system_clock::now();
            while (!utility::timedout(start, timeout)) {
                cv::Mat img = screenshot(icon);
                if (utility::count_matches(img, color, 30) > min_matches) {
                    return true;
                }
                checked_sleep(10ms);
            }
            return false;
        }
    }

    bool hud::is_extended_info_toggled() const
    {
        static cv::Rect roi{14, 34, 134, 35};
        return match(embedded::text::day, roi);
    }

    bool hud::is_player_overweight() const
    {
        return is_blinking(weight_icon_, blink_red_state_weight_, 400,
                           std::chrono::milliseconds(800));
    }

    bool hud::is_player_broken_bones() const
    {
        return is_blinking(health_icon_, blink_red_state_);
    }

    bool hud::is_player_out_of_water() const
    {
        return is_blinking(water_icon_, blink_red_state_);
    }

    bool hud::is_player_out_of_food() const
    {
        return is_blinking(food_icon_, blink_red_state_);
    }

    bool hud::is_player_sprinting() const { return false; }

    bool hud::can_default_teleport() const
    {
        return match(embedded::text::default_teleport, default_teleport_, 0.5);
    }

    bool hud::detected_enemy()
    {
        return detect_push_notification(embedded::text::detected_enemy);
    }

    bool hud::is_boss_teleport_in_active()
    {
        return detect_push_notification(embedded::text::teleport_in);
    }

    bool hud::is_boss_teleport_out_active()
    {
        return detect_push_notification(embedded::text::return_time_remaining, 0.85f);
    }

    bool hud::is_boss_on_cooldown()
    {
        return detect_push_notification(embedded::text::arena_available_in);
    }

    bool hud::is_boss_ongoing()
    {
        return detect_push_notification(embedded::text::arena_time_remaining, 0.85f);
    }

    bool hud::item_added(item& item, cv::Rect* roi_out) const
    {
        const cv::Rect roi = item_icon_removed_or_added_area;
        const std::vector<cv::Rect> locations = locate_all(
            item.get_notification_icon(), roi, 0.75f, false,
            item.get_notification_icon_mask());

        auto got_added = [roi, roi_out](const cv::Rect& r) -> bool {
            const auto loc = cv::Rect(roi.x + r.x + 20, roi.y + r.y - 10, 120, 25);
            if ((loc.x + loc.width) >= 1920 || (loc.y + loc.height > 1080)) {
                return false;
            }
            if (!item_added(loc)) { return false; }
            if (roi_out) { *roi_out = loc; }
            return true;
        };
        return std::ranges::any_of(locations, got_added);
    }

    bool hud::item_removed(item& item, cv::Rect* roi_out) const
    {
        const cv::Rect roi = item_icon_removed_or_added_area;
        const std::vector<cv::Rect> locations = locate_all(
            item.get_notification_icon(), roi, 0.65f, false,
            item.get_notification_icon_mask());

        auto got_removed = [roi, roi_out](const cv::Rect& r) -> bool {
            const auto loc = cv::Rect(roi.x + r.x + 20, roi.y + r.y - 10, 120, 30);
            if ((loc.x + loc.width) >= 1920 || (loc.y + loc.height > 1080)) {
                return false;
            }
            if (!item_removed(loc)) { return false; }
            if (roi_out) { *roi_out = loc; }
            return true;
        };
        return std::ranges::any_of(locations, got_removed);
    }

    bool hud::count_items_added(item& item, int& amount_out) const
    {
        cv::Rect roi{0, 0, 0, 0};
        if (!item_added(item, &roi)) { return false; }

        roi = {roi.x + 85, roi.y, 100, roi.height};

        auto x_loc = locate(embedded::text::x, roi);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location.\n";
            return false;
        }

        roi = {roi.x, roi.y, x_loc->x, roi.height};
        const cv::Mat img = screenshot(roi);
        const cv::Mat mask = utility::mask(img, cv::Vec3b{255, 255, 255}, 50);

        const std::string result_string = ocr_threadsafe(
            mask, tesseract::PSM_SINGLE_WORD, "0123456789");

        if (result_string.empty() || result_string == "\\n") {
            return false;
        }
        amount_out = std::stoi(result_string);
        return true;
    }

    bool hud::count_items_removed(item& item, int& amount_out) const
    {
        cv::Rect roi{0, 0, 0, 0};
        if (!item_removed(item, &roi)) { return false; }

        roi = {roi.x + 110, roi.y, 100, roi.height};

        auto x_loc = locate(embedded::text::x, roi);
        if (!x_loc.has_value()) {
            std::cerr << "[!] Failed to locate x location.\n";
            return false;
        }

        roi = {roi.x - 3, roi.y, x_loc->x, roi.height + 3};
        cv::Mat img = screenshot(roi);
        cv::Mat mask = utility::mask(img, cv::Vec3b{255, 255, 255}, 50);

        const std::string result_string = ocr_threadsafe(
            mask, tesseract::PSM_SINGLE_WORD, "0123456789");
        if (result_string.empty() || result_string == "\\n") {
            std::cerr << "[!] OCR failed, no result determined.\n";
            return false;
        }

        amount_out = std::stoi(result_string);
        return true;
    }

    void hud::toggle_extended(const bool on, const bool force)
    {
        if (!force) {
            // check if the hud is already in the state that was requested to avoid
            // desyncing it with the expected state.
            extended_toggled_ = is_extended_info_toggled();
            if (extended_toggled_ == on) { return; }
            extended_toggled_ ^= 1;
        }

        const action_mapping& keybind = get_action_mapping("ShowExtendedInfo");

        // differentiate between user setting 'toggle extended hud'
        if (get_user_setting<bool>("bToggleExtendedHUDInfo")) {
            post_press(keybind);
            return;
        }
        on ? post_down(keybind) : post_up(keybind);
    }

    bool hud::detect_push_notification(const cv::Mat& notification, const float variance)
    {
        if (match(notification, push_notifications_, variance)) {
            return true;
        }

        toggle_extended(true);
        checked_sleep(100ms);
        bool result = utility::await([this, &notification, variance] {
            return match(notification, push_notifications_, variance);
        }, 2s);

        toggle_extended(false);
        if (!result) {
            checked_sleep(100ms);
            result = utility::await([this, &notification, variance] {
                return match(notification, push_notifications_, variance);
            }, 2s);
        }
        return result;
    }

    bool hud::item_removed(const cv::Rect& area)
    {
        return match(embedded::text::removed, area);
    }

    bool hud::item_added(const cv::Rect& area)
    {
        return match(embedded::text::added, area);
    }

    bool hud::mount_has_level_up()
    {
        static cv::Vec3b blink_color{50, 224, 239};

        return is_blinking(dino_xp, blink_color, 70, std::chrono::milliseconds(700));
    }

    bool hud::mount_hud_available()
    {
        static cv::Vec3b text{255, 255, 255};
        static cv::Rect roi(1869, 48, 15, 10);

        const bool was_hud_toggled = is_extended_info_toggled();

        toggle_extended(true);
        checked_sleep(std::chrono::milliseconds(100));
        const cv::Mat mask = utility::mask(screenshot(roi), text, 20);

        // preserve the previous hud state if inteded
        if (!was_hud_toggled) { toggle_extended(false); }

        return cv::countNonZero(mask) > 20 || mount_has_level_up();
    }

    bool hud::is_mount_capped()
    {
        static cv::Vec3b black_weight{0, 0, 0};

        cv::Mat img = screenshot(dino_weightcapped);
        const cv::Mat mask = utility::mask(img, black_weight, 0);
        return cv::countNonZero(mask) > 950;
    }

    bool hud::is_player_capped()
    {
        static cv::Vec3b black_weight{0, 0, 0};

        cv::Mat img = screenshot(player_weightcapped);
        const cv::Mat mask = utility::mask(img, black_weight, 0);
        return cv::countNonZero(mask) > 950;
    }

    bool hud::can_harvest_target() const
    {
        cv::Vec3b color(123, 154, 155);

        cv::Mat img = screenshot(harvest_action_area);
        const cv::Mat mask = utility::mask(img, color, 10);
        return cv::countNonZero(mask) > 300;
    }

    hud* get_hud()
    {
        static auto instance = new hud();
        return instance;
    }
}
