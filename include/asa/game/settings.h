#pragma once
#include <any>
#include <map>
#include <filesystem>

namespace asa
{
    struct action_mapping
    {
        std::string name;
        std::string key;

        bool shift{false};
        bool ctrl{false};
        bool alt{false};
        bool cmd{false};
    };

    inline std::map<std::string, std::any> _user_settings_mappings;

    /**
     * @brief Loads the game settings from GameUserSettings.ini and Input.ini
     *
     * @param game_directory The path to the root directory of the game.
     */
    void load_game_settings(const std::filesystem::path& game_directory);

    template<typename T>
    T get_user_setting(const std::string& key)
    {
        return std::get<T>(_user_settings_mappings.at(key));
    }

    [[nodiscard]] const action_mapping& get_action_mapping(const std::string& key);
}
