#pragma once
#include "asa/items/item.h"
#include "asa/game/window.h"

namespace asa
{
    class hud
    {
    public:
        // Prevent copying
        hud(const hud&) = delete;

        hud& operator=(const hud&) = delete;

        [[nodiscard]] bool mount_hud_available();

        [[nodiscard]] bool mount_has_level_up();

        [[nodiscard]] bool is_mount_capped();

        [[nodiscard]] bool is_player_capped();

        [[nodiscard]] bool can_harvest_target() const;

        [[nodiscard]] bool is_player_overweight() const;

        [[nodiscard]] bool is_player_broken_bones() const;

        [[nodiscard]] bool is_player_out_of_water() const;

        [[nodiscard]] bool is_player_out_of_food() const;

        [[nodiscard]] bool is_player_sprinting() const;

        [[nodiscard]] bool can_default_teleport() const;

        [[nodiscard]] bool detected_enemy();

        [[nodiscard]] bool is_boss_teleport_in_active();

        [[nodiscard]] bool is_boss_teleport_out_active();

        [[nodiscard]] bool is_boss_on_cooldown();

        [[nodiscard]] bool is_boss_ongoing();

        [[nodiscard]] bool is_extended_info_toggled() const;

        /**
         * @brief Checks whether an item was removed in the given area
         * 
         * @param area The area to check whether an item was removed.
         * 
         * @return True if an item was removed, false otherwise.  
         */
        [[nodiscard]] static bool item_removed(const cv::Rect& area);

        /**
         * @brief Checks whether an item was added in the given area
         *
         * @param area The area to check whether an item was added.
         * 
         * @return True if an item was added, false otherwise.  
         */
        [[nodiscard]] static bool item_added(const cv::Rect& area);

        [[nodiscard]] bool item_added(item&, cv::Rect* roi_out) const;

        [[nodiscard]] bool item_removed(item&, cv::Rect* roi_out) const;

        [[nodiscard]] bool transferred_item_into_dedicated_storage() const;

        bool count_items_added(item&, int& amount_out) const;

        bool count_items_removed(item&, int& amount_out) const;

        float get_water_level() const;

        float get_food_level() const;

        float get_weight_level() const;

        float get_health_level() const;


        /**
         * @brief Toggles the extended hud information to a desired state.
         *
         * @param on True to turn the extended hud information on, otherwise false.
         * @param force Force toggling the state for situations where the hud cant be seen.
         *
         * @remark Respects the 'toggle extended hud' user setting.
         */
        void toggle_extended(bool on, bool force = false);

        friend std::shared_ptr<hud> get_hud();

    private:
        hud() = default;

        bool detect_push_notification(const cv::Mat& notification, float variance = 0.7f);

        cv::Vec3b blink_red_state_{109, 54, 52};
        cv::Vec3b blink_red_state_weight_{255, 45, 45};

        cv::Rect water_icon_{1868, 806, 34, 46};
        cv::Rect food_icon_{1861, 858, 46, 41};
        cv::Rect stamina_icon_{1872, 900, 29, 51};
        cv::Rect health_icon_{1861, 953, 48, 45};
        cv::Rect weight_icon_{1860, 751, 51, 56};
        cv::Rect default_teleport_{866, 911, 78, 27};
        cv::Rect push_notifications_{568, 0, 746, 1080};

        cv::Rect item_icon_removed_or_added_area{7, 12, 41, 1067};
        cv::Rect item_removed_area{43, 20, 110, 1054};
        cv::Rect item_added_area{40, 15, 85, 1063};
        cv::Rect status_update_area{844, 12, 224, 30};

        cv::Rect dino_xp{1872, 39, 9, 36};
        cv::Rect dino_weightcapped{1713, 33, 60, 33};
        cv::Rect player_weightcapped{1709, 993, 65, 58};
        cv::Rect harvest_action_area{1849, 676, 70, 78};

        bool extended_toggled_ = false;
    };

    std::shared_ptr<hud> get_hud();
}
