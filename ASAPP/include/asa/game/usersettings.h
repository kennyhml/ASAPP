#pragma once
#include <any>
#include <string>
#include <unordered_map>

namespace asa::settings
{
    inline static std::unordered_map<std::string, std::any> mapped_settings{};

    template<typename T>
    struct user_setting final
    {
        explicit user_setting(std::string t_name)
            : name(std::move(t_name)) { mapped_settings.emplace(name, T()); }

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
            return std::any_cast<T>(mapped_settings.at(name));
        }
    };

    enum FullscreenMode : int
    {
        FULLSCREEN = 0,
        WINDOWED_FULLSCREEN = 1,
        WINDOWED = 2,
    };

    inline user_setting<float> ui_scale("UIScaling");
    inline user_setting<float> hotbar_scale("UIQuickbarScaling");
    inline user_setting<float> camera_shake("CameraShakeScale");
    inline user_setting<float> fov("FOVMultiplier");
    inline user_setting<float> sens_x("LookLeftRightSensitivity");
    inline user_setting<float> sens_y("LookUpDownSensitivity");

    inline user_setting<bool> first_person_riding("bFirstPersonRiding");
    inline user_setting<bool> third_person("bThirdPersonPlayer");
    inline user_setting<bool> show_notis("bShowStatusNotificationMessages");
    inline user_setting<bool> toggle_hud("bToggleExtendedHUDInfo");
    inline user_setting<bool> inventory_tooltips("bEnableInventoryItemTooltips");

    inline user_setting<FullscreenMode> fullscreen_mode("FullscreenMode");
    inline user_setting<int> last_joined_category("LastSessionCategoryJoined");
    inline user_setting<int> remote_sort_type("RemoteItemSortType");

    inline user_setting<std::string> last_session_0("LastJoinedSessionPerCategory0");
    inline user_setting<std::string> last_session_1("LastJoinedSessionPerCategory1");
    inline user_setting<std::string> last_session_2("LastJoinedSessionPerCategory2");
    inline user_setting<std::string> last_session_3("LastJoinedSessionPerCategory3");
}
