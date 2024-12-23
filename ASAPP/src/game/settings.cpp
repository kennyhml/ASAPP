#include "asa/game/settings.h"
#include "asa/core/config.h"

#include <fstream>
#include <iostream>

namespace asa::settings
{
    namespace
    {
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
            }
            else if (key_out == "Key") {
                // Parse out the ending parantheses, example: Key=F)
                value_out = token.substr(eq + 1, token.length() - key_out.length() - 2);
            }
            else { value_out = token.substr(eq + 1); }

            return true;
        }

        std::any convert_settings_value(const std::string& key, const std::string& value)
        {
            if (key.find("LastJoinedSessionPer") != std::string::npos) { return value; }
            if (value.find('.') != std::string::npos) { return std::stof(value); }
            if (value == "True" || value == "False") { return value == "True"; }
            return std::stoi(value);
        }

        bool parse_user_settings(const std::istringstream& stream)
        {
            std::string key;
            std::string value;

            if (!parse_action_name(stream.str(), key, value)) { return false; }

            if (key == "LastJoinedSessionPerCategory") {
                key += std::to_string(session_category_count++);
            }

            if (!mapped_settings.contains(key)) { return false; }
            mapped_settings[key] = convert_settings_value(key, value);
            return true;
        }

        bool parse_action_mapping(std::string& from)
        {
            action_mapping* mapping = nullptr;
            std::istringstream stream(from);
            std::string token;
            std::vector<std::string> tokens;

            if (from.find("ActionMappings") != std::string::npos) {
                // skip first 16 characters for normal mappings, e.g 'ActionMappings=('
                stream.seekg(16);
                // collect all the tokens inside the action mapping
                while (std::getline(stream, token, ',')) { tokens.push_back(token); }
            }
            else if (from.find("ConsoleKeys") != std::string::npos) {
                tokens.push_back(from);
            }

            for (const std::string& tk : tokens) {
                std::string key;
                std::string value;

                if (!parse_action_name(tk, key, value)) { continue; }

                // get the pointer to the ActionMapping that we are parsing
                if (key == "ActionName") {
                    if (!action_mapping::mapped.contains(value)) { return false; }
                    mapping = action_mapping::mapped[value];
                    continue;
                }
                if (key == "ConsoleKeys") {
                    mapping = action_mapping::mapped[key];
                    mapping->key = value;
                    continue;
                }

                if (!mapping) {
                    std::cerr << std::format("[!] Couldn't parse '{}'", stream.str()) <<
                        "\n";
                    return false;
                }

                if (key == "bShift") { mapping->shift = (value == "True"); }
                else if (key == "bCtrl") { mapping->ctrl = (value == "True"); }
                else if (key == "bAlt") { mapping->alt = (value == "True"); }
                else if (key == "bCmd") { mapping->cmd = (value == "True"); }
                else if (key == "Key") { mapping->key = value; }
            }

            if (!mapping) { return false; }
            return true;
        }
    }

    bool load_action_mappings()
    {
        std::ifstream file;
        if (!open_file(core::config::game_base_directory / INPUT_SETTINGS_REL, file)) {
            return false;
        }

        for (std::string line; std::getline(file, line);) {
            parse_action_mapping(line);
        }
        return true;
    }

    bool load_user_settings()
    {
        std::ifstream file;
        if (!open_file(core::config::game_base_directory / USER_SETTINGS_REL, file)) {
            return false;
        }

        bool section_found = false;
        session_category_count = 0;

        for (std::string line; std::getline(file, line);) {
            const bool section_started = line.find("ShooterGame") != std::string::npos;
            if (line.find("ServerSettings") != std::string::npos) { break; }

            if (!section_found && !section_started) { continue; }
            if (section_started) {
                section_found = true;
                continue;
            }

            std::istringstream ss(line);
            parse_user_settings(ss);
        }
        return true;
    }

    void load()
    {
        load_user_settings();
        load_action_mappings();
    }
}
