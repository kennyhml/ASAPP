#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <any>

namespace asa::settings
{
    bool init();

    inline namespace action_mappings
    {
        bool load_action_mappings(bool verbose = true);

        struct ActionMapping;
        inline std::unordered_map<std::string, ActionMapping*> input_map{};

        struct ActionMapping final
        {
            ActionMapping(std::string t_name, std::string t_default);

            std::string name;
            std::string key;

            bool shift{false};
            bool ctrl{false};
            bool alt{false};
            bool cmd{false};
        };

        std::ostream& operator<<(std::ostream& os, const ActionMapping& m);

        inline ActionMapping access_inventory("AccessInventory", "F");
        inline ActionMapping crouch("Crouch", "C");
        inline ActionMapping prone("Prone", "X");
        inline ActionMapping orbit_cam("OrbitCam", "K");
        inline ActionMapping pause_menu("PauseMenu", "Escape");
        inline ActionMapping ping("Ping", "MiddleMouseButton");
        inline ActionMapping poop("Poop", "Semicolon");
        inline ActionMapping reload("Reload", "R");
        inline ActionMapping show_tribe_manager("ShowTribeManager", "L");
        inline ActionMapping toggle_map("ToggleMap", "M");
        inline ActionMapping transfer_item("TransferItem", "T");
        inline ActionMapping use("Use", "E");
        inline ActionMapping use_item1("UseItem1", "1");
        inline ActionMapping use_item2("UseItem2", "2");
        inline ActionMapping use_item3("UseItem3", "3");
        inline ActionMapping use_item4("UseItem4", "4");
        inline ActionMapping use_item5("UseItem5", "5");
        inline ActionMapping use_item6("UseItem6", "6");
        inline ActionMapping use_item7("UseItem7", "7");
        inline ActionMapping use_item8("UseItem8", "8");
        inline ActionMapping use_item9("UseItem9", "9");
        inline ActionMapping use_item10("UseItem10", "0");
        inline ActionMapping show_my_inventory("ShowMyInventory", "I");
        inline ActionMapping drop_item("DropItem", "O");
        inline ActionMapping fire("Fire", "LeftMouseButton");
        inline ActionMapping targeting("Targeting", "RightMouseButton");
        inline ActionMapping show_extended_info("ShowExtendedInfo", "H");
        inline ActionMapping toggle_tooltip("ToggleTooltip", "G");
        inline ActionMapping jump("Jump", "Space");
        inline ActionMapping run("Run", "LeftShift");
        inline ActionMapping console("ConsoleKeys", "Tilde");
    }

    inline namespace game_user_settings
    {
        inline std::unordered_map<std::string, std::any> setting_value_map{};

        template <typename T>
        struct UserSetting final
        {
            explicit UserSetting(std::string t_name): name(std::move(t_name))
            {
                setting_value_map[name] = T();
            }

            std::string name;

            bool operator==(T other) const { return get() == other; }

            /**
             * @brief Retrieves the value of the setting.
             *
             * @tparam T The type of the setting value.
             * @return The value of the setting.
             */
            [[nodiscard]] T get() const
            {
                return std::any_cast<T>(setting_value_map.at(name));
            }
        };

        enum FullscreenMode : int
        {
            FULLSCREEN = 0,
            WINDOWED_FULLSCREEN = 1,
            WINDOWED = 2,
        };

        bool load_user_settings(bool verbose = true);

        inline UserSetting<float> ui_scale("UIScaling");
        inline UserSetting<float> hotbar_scale("UIQuickbarScaling");
        inline UserSetting<float> camera_shake("CameraShakeScale");
        inline UserSetting<float> fov("FOVMultiplier");
        inline UserSetting<float> sens_x("LookLeftRightSensitivity");
        inline UserSetting<float> sens_y("LookUpDownSensitivity");

        inline UserSetting<bool> first_person_riding("bFirstPersonRiding");
        inline UserSetting<bool> third_person("bThirdPersonPlayer");
        inline UserSetting<bool> show_notis("bShowStatusNotificationMessages");
        inline UserSetting<bool> toggle_hud("bToggleExtendedHUDInfo");

        inline UserSetting<int> fullscreen_mode("FullscreenMode");
        inline UserSetting<int> last_joined_category("LastSessionCategoryJoined");

        inline UserSetting<std::string> last_session_0("LastJoinedSessionPerCategory0");
        inline UserSetting<std::string> last_session_1("LastJoinedSessionPerCategory1");
        inline UserSetting<std::string> last_session_2("LastJoinedSessionPerCategory2");
        inline UserSetting<std::string> last_session_3("LastJoinedSessionPerCategory3");
    }
}
