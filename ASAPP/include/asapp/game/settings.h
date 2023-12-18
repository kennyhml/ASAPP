#pragma once
#define WIN32_LEAN_AND_MEAN

#include <any>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <Windows.h>

namespace asa::settings
{
    bool init();
    bool open_file(bool verbose, std::filesystem::path path, std::ifstream& fileOut);

    inline namespace action_mappings
    {
        struct ActionMapping;
        inline std::unordered_map<std::string, ActionMapping*> input_map{};

        struct ActionMapping
        {
            explicit ActionMapping(std::string t_name) : name(t_name)
            {
                input_map[name] = this;
            };

            std::string name;
            bool shift{false}, ctrl{false}, alt{false}, cmd{false};
            std::string key;
        };

        std::ostream& operator<<(std::ostream& os, const ActionMapping& m);

        const auto inputs_rel_path = std::filesystem::path(
            R"(ShooterGame\Saved\Config\Windows\Input.ini)");

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

        bool load_action_mappings(bool verbose = true);
    }

    inline namespace game_user_settings
    {
        inline std::unordered_map<std::string, std::any> setting_value_map{};

        template <typename Type>
        struct UserSetting
        {
            UserSetting(std::string t_name) : name(t_name)
            {
                setting_value_map[name] = Type(0);
            };

            std::string name;

            Type get() { return std::any_cast<Type>(setting_value_map.at(name)); }
        };

        const auto user_settings_rel_path = std::filesystem::path(
            R"(ShooterGame\Saved\Config\Windows\GameUserSettings.ini)");

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
    }
}
