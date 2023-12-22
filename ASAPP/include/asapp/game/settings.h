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
            explicit ActionMapping(std::string t_name);

            std::string name;
            std::string key;

            bool shift{false};
            bool ctrl{false};
            bool alt{false};
            bool cmd{false};
        };

        std::ostream& operator<<(std::ostream& os, const ActionMapping& m);

        inline ActionMapping access_inventory("AccessInventory");
        inline ActionMapping crouch("Crouch");
        inline ActionMapping prone("Prone");
        inline ActionMapping orbit_cam("OrbitCam");
        inline ActionMapping pause_menu("PauseMenu");
        inline ActionMapping ping("Ping");
        inline ActionMapping poop("Poop");
        inline ActionMapping reload("Reload");
        inline ActionMapping show_tribe_manager("ShowTribeManager");
        inline ActionMapping toggle_map("ToggleMap");
        inline ActionMapping transfer_item("TransferItem");
        inline ActionMapping use("Use");
        inline ActionMapping use_item1("UseItem1");
        inline ActionMapping use_item2("UseItem2");
        inline ActionMapping use_item3("UseItem3");
        inline ActionMapping use_item4("UseItem4");
        inline ActionMapping use_item5("UseItem5");
        inline ActionMapping use_item6("UseItem6");
        inline ActionMapping use_item7("UseItem7");
        inline ActionMapping use_item8("UseItem8");
        inline ActionMapping use_item9("UseItem9");
        inline ActionMapping use_item10("UseItem10");
        inline ActionMapping show_my_inventory("ShowMyInventory");
        inline ActionMapping drop_item("DropItem");
        inline ActionMapping fire("Fire");
        inline ActionMapping targeting("Targeting");
        inline ActionMapping show_extended_info("ShowExtendedInfo");
        inline ActionMapping toggle_tooltip("ToggleTooltip");
        inline ActionMapping jump("Jump");
        inline ActionMapping run("Run");
    }

    inline namespace game_user_settings
    {
        inline std::unordered_map<std::string, std::any> setting_value_map{};

        template <typename T>
        struct UserSetting final
        {
            explicit UserSetting(std::string t_name): name(std::move(t_name))
            {
                setting_value_map[name] = T(0);
            }

            std::string name;

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
    }
}
