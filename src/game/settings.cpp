#include "asa/game/settings.h"

#include <fstream>
#include <iostream>

namespace asa
{
    namespace
    {
        std::map<std::string, std::unique_ptr<action_mapping> > action_mappings;

        int session_category_count = 0;

        const auto USER_SETTINGS_REL = std::filesystem::path(
            R"(ShooterGame\Saved\Config\Windows\GameUserSettings.ini)");

        const auto INPUT_SETTINGS_REL = std::filesystem::path(
            R"(ShooterGame\Saved\Config\Windows\Input.ini)");

        bool open_file(const std::filesystem::path& path, std::ifstream& out_file)
        {
            if (!exists(path)) {
                std::cout << std::format("[!] Path '{}' was not found.", path.string());
                return false;
            }
            out_file.open(path.c_str());
            if (!out_file.is_open()) {
                std::cerr << std::format("[!] Couldn't open '{}'", path.string()) << "\n";
                return false;
            }
            return true;
        }

        bool parse_action_name(const std::string& token, std::string& key_out,
                               std::string& value_out)
        {
            const size_t eq = token.find('=');
            if (eq == std::string::npos) { return false; }

            key_out = token.substr(0, eq);
            // Parse out the value, example: ActionName="AccessInventory"
            // Exclude  = and " characters from the key  ---------------
            if (key_out == "ActionName") {
                value_out = token.substr(eq + 2, token.length() - (key_out.length() + 3));
            } else if (key_out == "Key") {
                // Parse out the ending parantheses, example: Key=F)
                value_out = token.substr(eq + 1, token.length() - key_out.length() - 2);
            } else { value_out = token.substr(eq + 1); }

            return true;
        }

        std::any convert_settings_value(const std::string& key, const std::string& value)
        {
            if (key.contains("LastJoinedSessionPer")) { return value; }
            if (value.contains('.')) { return std::stof(value); }
            if (value == "True" || value == "False") { return value == "True"; }
            return std::stoi(value);
        }

        void parse_user_settings(const std::istringstream& stream)
        {
            std::string key;
            std::string value;

            if (!parse_action_name(stream.str(), key, value)) { return; }

            if (key == "LastJoinedSessionPerCategory") {
                key += std::to_string(session_category_count++);
            }

            _user_settings_mappings[key] = convert_settings_value(key, value);
        }

        /**
         * @brief Parses an action mapping, for example:
         * ActionMappings=(ActionName="Reload",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=R)
         *
         * The mapping is split into its indivual tokens, e.g
         * ActionName: Reload
         * bShift: False
         * bCtrl: False
         * bAlt: False
         * bCmd: False
         * Key: R
         *
         * With the action names value "Reload" being the key to later access the mapping.
         *
         * @remark There are special cases, for example the console keys are just stored
         * as "ConsoleKeys=Tilde" with no further data.
         *
         * @param from The action mapping to parse.
         */
        void parse_action_mapping(const std::string& from)
        {
            action_mapping* mapping = nullptr;
            std::istringstream stream(from);
            std::vector<std::string> tokens;

            if (from.contains("ActionMappings")) {
                // skip first 16 characters for normal mappings, e.g 'ActionMappings=('
                stream.seekg(16);

                // collect all the tokens inside the action mapping
                std::string token;
                while (std::getline(stream, token, ',')) { tokens.push_back(token); }
            } else if (from.contains("ConsoleKeys")) {
                tokens.push_back(from);
            }

            for (const std::string& tk: tokens) {
                std::string key;
                std::string value;

                if (!parse_action_name(tk, key, value)) { continue; }

                // get the pointer to the ActionMapping that we are parsing
                if (key == "ActionName") {
                    if (!action_mappings.contains(value)) {
                        action_mappings[value] = std::make_unique<action_mapping>();
                    }
                    mapping = action_mappings.at(value).get();
                    continue;
                }
                if (key == "ConsoleKeys") {
                    action_mappings[key] = std::make_unique<action_mapping>();
                    action_mappings[key]->key = value;
                    return;
                }

                // if we dont have a pointer to the action mapping at this point
                // then we cant assign the values either
                if (!mapping) { return; }

                if (key == "bShift") {
                    mapping->shift = (value == "True");
                } else if (key == "bCtrl") {
                    mapping->ctrl = (value == "True");
                } else if (key == "bAlt") {
                    mapping->alt = (value == "True");
                } else if (key == "bCmd") {
                    mapping->cmd = (value == "True");
                } else if (key == "Key") { mapping->key = value; }
            }
        }
    }

    void load_action_mappings(const std::filesystem::path& game_directory)
    {
        std::ifstream file;
        if (!open_file(game_directory / INPUT_SETTINGS_REL, file)) {
            return;
        }

        for (std::string line; std::getline(file, line);) {
            parse_action_mapping(line);
        }
    }

    void load_user_settings(const std::filesystem::path& game_directory)
    {
        std::ifstream file;
        if (!open_file(game_directory / USER_SETTINGS_REL, file)) {
            return;
        }

        bool section_found = false;
        session_category_count = 0;

        for (std::string line; std::getline(file, line);) {
            const bool section_started = line.contains("ShooterGame");
            if (line.contains("ServerSettings")) { break; }

            if (!section_found && !section_started) { continue; }
            if (section_started) {
                section_found = true;
                continue;
            }

            std::istringstream ss(line);
            parse_user_settings(ss);
        }
    }

    void load_game_settings(const std::filesystem::path& game_directory)
    {
        load_user_settings(game_directory);
        load_action_mappings(game_directory);
    }

    const action_mapping& get_action_mapping(const std::string& key)
    {
        return *action_mappings.at(key);
    }
}
