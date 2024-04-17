#include "asapp/interfaces/hud.h"
#include "asapp/util/util.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include "asapp/core/state.h"
#include "asapp/game/resources.h"
#include "asapp/game/window.h"

namespace asa::interfaces
{
    namespace
    {
        /**
         * @brief This function helps optimize the template match on all interactions
         * that are underlined by the cyan line which will appear when one or more options
         * are available on a structure.
         *
         * @return The roi to look for the interaction text in if found, else std::nullopt.
         */
        std::optional<cv::Rect> find_multi_interactable_line()
        {
            static constexpr window::Color cyan{0, 255, 255};
            const auto ss = window::screenshot();
            const auto mask = window::get_mask(ss, cyan, 10);

            std::vector<std::vector<cv::Point> > contours;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            double longest = 0;
            cv::Rect best_contour;
            for (const auto& contour: contours) {
                const auto rect = cv::boundingRect(contour);
                // not the shape we are looking for.
                if (rect.width < rect.height || rect.width < 150) { continue; }

                if (const auto length = cv::arcLength(contour, true); length > longest) {
                    longest = length;
                    best_contour = rect;
                }
            }
            return best_contour.empty() ? std::nullopt : std::optional(best_contour);
        }

        bool is_blinking(const window::Rect& icon, const window::Color& color,
                         int min_matches = 500,
                         const std::chrono::milliseconds timeout =
                                 std::chrono::milliseconds(500))
        {
            const auto start = std::chrono::system_clock::now();
            while (!util::timedout(start, timeout)) {
                cv::Mat mask = window::get_mask(icon, color, 30);
                if (cv::countNonZero(mask) > min_matches) { return true; }
            }
            return false;
        }

        cv::Rect find_max_contour(const std::vector<std::vector<cv::Point> >& contours)
        {
            int max_size = 0;
            cv::Rect max_cont;

            for (const auto& contour: contours) {
                if (contourArea(contour) > max_size) {
                    max_size = cv::contourArea(contour);
                    max_cont = boundingRect(contour);
                }
            }

            return max_cont;
        }
    }

    bool HUD::extended_information_is_toggled() const
    {
        static window::Rect roi{14, 34, 134, 35};
        return window::match_template(roi, resources::text::day);
    }

    bool HUD::is_player_overweight() const
    {
        return is_blinking(weight_icon_, blink_red_state_weight_, 400,
                           std::chrono::milliseconds(800));
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

    bool HUD::can_fast_travel()
    {
        const auto narrowed = find_multi_interactable_line();
        if (!narrowed.has_value()) { return false; }

        const window::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};
        return window::match_template(roi, resources::text::fast_travel);
    }

    bool HUD::can_teleport()
    {
        const auto narrowed = find_multi_interactable_line();
        if (!narrowed.has_value()) { return false; }

        const window::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};
        return window::match_template(roi, resources::text::teleport_to);
    }

    bool HUD::can_access_inventory() const
    {
        const auto narrowed = find_multi_interactable_line();
        if (!narrowed.has_value()) { return false; }

        const window::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};
        return window::match_template(roi, resources::text::access_inventory);
    }

    bool HUD::can_ride() const
    {
        const auto narrowed = find_multi_interactable_line();
        if (!narrowed.has_value()) { return false; }

        const window::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};
        return window::match_template(roi, resources::text::access_inventory, 0.85f);
    }

    bool HUD::can_pick_up() const
    {
        const auto narrowed = find_multi_interactable_line();
        if (!narrowed.has_value()) { return false; }

        const window::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};
        return window::match_template(roi, resources::text::pick_up);
    }

    bool HUD::detected_enemy()
    {
        return detect_push_notification(resources::text::detected_enemy);
    }

    bool HUD::is_boss_teleport_in_active()
    {
        return detect_push_notification(resources::text::teleport_in);
    }

    bool HUD::is_boss_teleport_out_active()
    {
        return detect_push_notification(resources::text::return_time_remaining, 0.85f);
    }

    bool HUD::is_boss_on_cooldown()
    {
        return detect_push_notification(resources::text::arena_available_in);
    }

    bool HUD::is_boss_ongoing()
    {
        return detect_push_notification(resources::text::arena_time_remaining, 0.85f);
    }

    bool HUD::item_added(items::Item& item, window::Rect* roi_out) const
    {
        const window::Rect roi = item_icon_removed_or_added_area;
        const std::vector<window::Rect> locations = window::locate_all_template(
            roi, item.get_notification_icon(), 0.75f, item.get_notification_icon_mask());

        auto got_added = [roi, roi_out](const window::Rect& r) -> bool {
            const auto loc = window::Rect(roi.x + r.x + 20, roi.y + r.y - 10, 120, 25);
            if ((loc.x + loc.width) >= 1920 || (loc.y + loc.height > 1080)) {
                return false;
            }
            if (!item_added(loc)) { return false; }
            if (roi_out) { *roi_out = loc; }
            return true;
        };
        return std::ranges::any_of(locations, got_added);
    }

    bool HUD::item_removed(items::Item& item, window::Rect* roi_out) const
    {
        const window::Rect roi = item_icon_removed_or_added_area;
        const std::vector<window::Rect> locations = window::locate_all_template(
            roi, item.get_notification_icon(), 0.65f, item.get_notification_icon_mask());

        auto got_removed = [roi, roi_out](const window::Rect& r) -> bool {
            const auto loc = window::Rect(roi.x + r.x + 20, roi.y + r.y - 10, 120, 30);
            if ((loc.x + loc.width) >= 1920 || (loc.y + loc.height > 1080)) {
                return false;
            }
            if (!item_removed(loc)) { return false; }
            if (roi_out) { *roi_out = loc; }
            return true;
        };
        return std::ranges::any_of(locations, got_removed);
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

        roi = {roi.x - 3, roi.y, x_loc->x, roi.height + 3};
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

    void HUD::toggle_extended(const bool on, const bool force)
    {
        if (!force) {
            // check if the hud is already in the state that was requested to avoid
            // desyncing it with the expected state.
            extended_toggled_ = extended_information_is_toggled();
            if (extended_toggled_ == on) { return; }
            extended_toggled_ ^= 1;
        }

        // differentiate between user setting 'toggle extended hud'
        if (settings::toggle_hud.get()) {
            return window::press(settings::show_extended_info);
        }
        if (on) { window::down(settings::show_extended_info); } else {
            window::up(settings::show_extended_info);
        }
    }

    bool HUD::detect_push_notification(const cv::Mat& notification, const float variance)
    {
        if (window::match_template(push_notifications_, notification, variance)) {
            return true;
        }

        toggle_extended(true);
        core::sleep_for(100ms);
        bool result = util::await([this, &notification, variance] {
            return match_template(push_notifications_, notification, variance);
        }, 2s);

        toggle_extended(false);
        if (!result) {
            core::sleep_for(100ms);
            result = util::await([this, &notification, variance] {
                return match_template(push_notifications_, notification, variance);
            }, 2s);
        }
        return result;
    }

    float HUD::get_health_level() const
    {
        static window::Rect roi(1881, 956, 9, 42);
        const cv::Mat original_img = window::screenshot(roi);

        cv::Mat grayscale;
        cv::cvtColor(original_img, grayscale, cv::COLOR_RGB2GRAY);
        double variance = 0.0;
        cv::Scalar mean, stddev;
        cv::meanStdDev(grayscale, mean, stddev);
        variance = stddev.val[0] * stddev.val[0];

        cv::Mat pix = original_img.reshape(1, original_img.total());
        // k-means requires float32
        pix.convertTo(pix, CV_32F);

        const cv::TermCriteria criteria(
            cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2);
        const int clusters = variance > 50.f ? 2 : 1;
        cv::Mat labels;
        cv::Mat centers;
        cv::kmeans(pix, clusters, labels, criteria, 3, cv::KMEANS_RANDOM_CENTERS,
                   centers);

        centers.convertTo(centers, CV_8U);
        cv::Mat mask1 = cv::Mat::zeros(original_img.size(), CV_8U);
        cv::Mat mask2 = cv::Mat::zeros(original_img.size(), CV_8U);

        for (int i = 0; i < pix.rows; ++i) {
            if (labels.at<int>(i, 0)) { mask1.at<uchar>(i) = 255; } else {
                mask2.at<uchar>(i) = 255;
            }
        }

        cv::Mat structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::morphologyEx(mask1, mask1, cv::MORPH_CLOSE, structure);
        cv::morphologyEx(mask1, mask1, cv::MORPH_OPEN, structure);
        cv::morphologyEx(mask2, mask2, cv::MORPH_CLOSE, structure);
        cv::morphologyEx(mask2, mask2, cv::MORPH_OPEN, structure);

        using contour = std::vector<cv::Point>;
        std::vector<contour> contours1, contours2;

        cv::findContours(mask1, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(mask2, contours2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // get rid of all the noise and make sure we only got our clean rects
        cv::Rect m1 = find_max_contour(contours1);
        m1.width = roi.width;
        mask1.setTo(cv::Scalar(0, 0, 0));
        cv::rectangle(mask1, m1, {255, 255, 255}, -1);


        cv::Rect m2 = find_max_contour(contours2);
        m2.width = roi.width;
        mask2.setTo(cv::Scalar(0, 0, 0));
        cv::rectangle(mask2, m2, {255, 255, 255}, -1);


        // make sure there is no overlap between the masks
        if (m1.area() != roi.height * roi.width && (m2.area() != roi.height * roi.
                                                    width)) { mask2 = mask2 & ~mask1; }


        // lets look at the bigger of the two masks only as it's usually more reliable.
        cv::Mat relevant_mask = m1.area() > m2.area() ? mask1 : mask2;
        return 0.f;
    }

    bool HUD::item_removed(const window::Rect& area)
    {
        return window::match_template(area, resources::text::removed);
    }

    bool HUD::item_added(const window::Rect& area)
    {
        return window::match_template(area, resources::text::added);
    }

    bool HUD::mount_has_level_up()
    {
        static constexpr window::Color blink_color{50, 224, 239};

        return is_blinking(dino_xp, blink_color, 70, std::chrono::milliseconds(700));
    }

    bool HUD::mount_hud_available()
    {
        static constexpr window::Color text{255, 255, 255};
        static window::Rect roi(1869, 48, 15, 10);

        const bool was_hud_toggled = extended_information_is_toggled();

        toggle_extended(true);
        asa::core::sleep_for(std::chrono::milliseconds(100));
        const cv::Mat mask = window::get_mask(roi, text, 20);

        // preserve the previous hud state if inteded
        if (!was_hud_toggled) { toggle_extended(false); }

        return cv::countNonZero(mask) > 20 || mount_has_level_up();
    }

    bool HUD::is_mount_capped()
    {
        static constexpr window::Color black_weight{0, 0, 0};

        const cv::Mat mask = window::get_mask(dino_weightcapped, black_weight, 0);
        return cv::countNonZero(mask) > 950;
    }

    bool HUD::is_player_capped()
    {
        static constexpr window::Color black_weight{0, 0, 0};

        const cv::Mat mask = window::get_mask(player_weightcapped, black_weight, 0);
        return cv::countNonZero(mask) > 950;
    }

    bool HUD::can_harvest_target() const
    {
        constexpr window::Color color(123, 154, 155);
        const cv::Mat mask = window::get_mask(harvest_action_area, color, 10);
        return cv::countNonZero(mask) > 300;
    }
}
