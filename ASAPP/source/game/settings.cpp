#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
#include "asapp/game/settings.h"
#include "asapp/core/config.h"

#define VERBOSE_LOG(log)         \
    if (verbose) { std::cout << log << "\n"; }

namespace asa::settings
{
    namespace
    {
        const auto USER_SETTINGS_REL = std::filesystem::path(
                R"(ShooterGame\Saved\Config\Windows\GameUserSettings.ini)");

        const auto INPUT_SETTINGS_REL = std::filesystem::path(
                R"(ShooterGame\Saved\Config\Windows\Input.ini)");

        bool open_file(const std::filesystem::path& path, std::ifstream& out_file)
        {
            if (!std::filesystem::exists(path)) {
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
            if (key == "LastJoinedSessionPerCategory") { return value; }
            if (value.find('.') != std::string::npos) { return std::stof(value); }
            if (value == "True" || value == "False") { return value == "True"; }
            return std::stoi(value);
        }

        bool parse_user_settings(const std::istringstream& stream, const bool verbose)
        {
            auto& map = setting_value_map;
            std::string key;
            std::string value;

            if (!parse_action_name(stream.str(), key, value)) { return false; }
            if (!map.contains(key)) { return false; }
            map[key] = convert_settings_value(key, value);
            VERBOSE_LOG("\t[-] Parsed " << key << " (" << value << ")")
            return true;
        }

        bool parse_action_mapping(std::string& from, const bool verbose)
        {
            ActionMapping* mapping = nullptr;
            std::istringstream stream(from);
            std::string token;
            std::vector<std::string> tokens;

            if (from.find("ActionMappings") != std::string::npos) {
                // skip first 16 characters for normal mappings, e.g 'ActionMappings=('
                stream.seekg(16);
                // collect all the tokens inside the action mapping
                while (std::getline(stream, token, ',')) { tokens.push_back(token); }
            } else if (from.find("ConsoleKeys") != std::string::npos) {
                tokens.push_back(from);
            }

            for (const std::string& tk: tokens) {
                std::string key;
                std::string value;

                if (!parse_action_name(tk, key, value)) { continue; }

                // get the pointer to the ActionMapping that we are parsing
                if (key == "ActionName") {
                    if (!input_map.contains(value)) { return false; }
                    mapping = input_map[value];
                    continue;
                } else if (key == "ConsoleKeys") {
                    mapping = input_map[key];
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
            VERBOSE_LOG("\t[-] Parsed " << *mapping)
            return true;
        }
    }

    bool init()
    { return load_user_settings() && load_action_mappings(); }

    ActionMapping::ActionMapping(std::string t_name, std::string t_default) : name(
            std::move(t_name)), key(std::move(t_default))
    {
        input_map[name] = this;
    };

    std::ostream& action_mappings::operator
    <<(std::ostream& os, const action_mappings::ActionMapping& m)
    {
        return os << std::format(
                "ActionMapping(name={}, shift={}, " "ctrl={}, alt={}, cmd={}, key={})",
                m.name, static_cast<int>(m.shift), static_cast<int>(m.ctrl),
                static_cast<int>(m.alt), static_cast<int>(m.cmd), m.key);
    }

    bool action_mappings::load_action_mappings(const bool verbose)
    {
        std::ifstream file;
        if (!open_file(core::config::game_base_directory / INPUT_SETTINGS_REL, file)) {
            return false;
        }

        VERBOSE_LOG("[+] Parsing Input.ini...")
        for (std::string line; std::getline(file, line);) {
            parse_action_mapping(line, verbose);
        }
        VERBOSE_LOG("[+] Input.ini parsed, mappings mapped.")
        return true;
    }

    bool game_user_settings::load_user_settings(const bool verbose)
    {
        std::ifstream file;
        if (!open_file(core::config::game_base_directory / USER_SETTINGS_REL, file)) {
            return false;
        }

        VERBOSE_LOG("[+] Parsing GameUserSettings.ini...")
        bool section_found = false;

        for (std::string line; std::getline(file, line);) {
            const bool section_started = line.find("ShooterGame") != std::string::npos;
            if (line.find("ServerSettings") != std::string::npos) { break; }

            if (!section_found && !section_started) { continue; }
            if (section_started) {
                section_found = true;
                continue;
            }

            std::istringstream ss(line);
            parse_user_settings(ss, verbose);
        }
        VERBOSE_LOG("[+] GameUserSettings.ini parsed.")
        return true;
    }
}
