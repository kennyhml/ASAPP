#include "asapp/game/settings.h"
#include <format>
#include <fstream>
#include <sstream>
#include "asapp/core/config.h"
#include "asapp/game/globals.h"

#define VERBOSE_LOG(log)                                                       \
	if (verbose) {                                                             \
		std::cout << log << std::endl;                                         \
	}

namespace asa::settings
{
    bool init() { return load_user_settings() && load_action_mappings(); }

    bool open_file(bool verbose, std::filesystem::path path, std::ifstream& out_file)
    {
        if (!std::filesystem::exists(path)) {
            std::cout << std::format("[!] Path '{}' was not found.", path.string());
            return false;
        }

        out_file.open(path.c_str());
        if (!out_file.is_open()) {
            std::cout << std::format("[!] Failed to open '{}'", path.string()) <<
                std::endl;
            return false;
        }

        return true;
    }

    std::ostream& action_mappings::operator
    <<(std::ostream& os, const action_mappings::ActionMapping& m)
    {
        return os << std::format(
            "ActionMapping(name={}, shift={}, " "ctrl={}, alt={}, cmd={}, key={})",
            m.name, static_cast<int>(m.shift), static_cast<int>(m.ctrl),
            static_cast<int>(m.alt), static_cast<int>(m.cmd), m.key);
    }

    static bool parse_key_value(std::string token, std::string& key_out,
                                std::string& value_out)
    {
        size_t eq = token.find('=');
        if (eq == std::string::npos) { return false; }

        key_out = token.substr(0, eq);
        // Parse out the value, example: ActionName="AccessInventory"
        // Exclude  = and " characters from the key  ---------------
        if (key_out == "ActionName") {
            value_out = token.substr(eq + 2, token.length() - (key_out.length() + 3));
        }
        // Parse out the ending parantheses, example: Key=F)
        else if (key_out == "Key") {
            value_out = token.substr(eq + 1, token.length() - key_out.length() - 2);
        }
        else { value_out = token.substr(eq + 1); }
        return true;
    }

    static std::any convert_settings_value(std::string key, std::string value)
    {
        if (key == "LastJoinedSessionPerCategory") { return value; }
        if (value.find(".") != std::string::npos) { return std::stof(value); }
        if (value == "True" || value == "False") { return value == "True"; }
        return std::stoi(value);
    }

    static bool parse_user_settings(std::istringstream& stream, bool verbose)
    {
        auto& map = game_user_settings::setting_value_map;
        std::string key;
        std::string value;

        if (!parse_key_value(stream.str(), key, value)) { return false; }
        if (!map.contains(key)) { return false; }
        map[key] = convert_settings_value(key, value);
        VERBOSE_LOG("\t[-] Parsed " << key << " (" << value << ")");
        return true;
    }

    static bool parse_action_mappings(std::istringstream& stream, bool verbose)
    {
        auto& map = action_mappings::input_map;

        // skip the first 16 characters 'ActionMappings=('
        stream.seekg(16);

        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(stream, token, ',')) { tokens.push_back(token); }

        ActionMapping* mapping = nullptr;

        for (const std::string& token : tokens) {
            std::string key;
            std::string value;
            if (!parse_key_value(token, key, value)) { continue; }

            if (key == "ActionName") {
                if (!map.contains(value)) {
                    return false; // we only care for some action mappings
                }
                mapping = map[value];
                continue;
            }

            if (!mapping) {
                std::cout << std::format("[!] Unable to parse '{}'", stream.str()) <<
                    std::endl;
                return false;
            }

            if (key == "bShift") { mapping->shift = (value == "True"); }
            else if (key == "bCtrl") { mapping->ctrl = (value == "True"); }
            else if (key == "bAlt") { mapping->alt = (value == "True"); }
            else if (key == "bCmd") { mapping->cmd = (value == "True"); }
            else if (key == "Key") { mapping->key = value; }
        }
        VERBOSE_LOG("\t[-] Parsed " << *mapping)
        return true;
    }

    bool action_mappings::load_action_mappings(bool verbose)
    {
        std::ifstream file;
        if (!open_file(verbose, core::config::game_base_directory / inputs_rel_path,
                       file)) { return false; }

        VERBOSE_LOG("[+] Parsing Input.ini...");
        for (std::string line; std::getline(file, line);) {
            if (line.find("ActionMappings=") != std::string::npos) {
                std::istringstream ss(line);
                parse_action_mappings(ss, verbose);
            }
        }
        VERBOSE_LOG("[+] Input.ini parsed, mappings mapped.")
        return true;
    }

    bool game_user_settings::load_user_settings(bool verbose)
    {
        std::ifstream file;
        auto path = core::config::game_base_directory / user_settings_rel_path;

        if (!open_file(verbose, path, file)) { return false; }

        VERBOSE_LOG("[+] Parsing GameUserSettings.ini...")
        bool section_found = false;

        for (std::string line; std::getline(file, line);) {
            bool section_start = line.find("ShooterGame") != std::string::npos;
            if (line.find("ServerSettings") != std::string::npos) { break; }

            if (!section_found && !section_start) { continue; }

            if (section_start) {
                section_found = true;
                continue;
            }

            std::istringstream ss(line);
            parse_user_settings(ss, verbose);
        }
        VERBOSE_LOG("[+] GameUserSettings.ini parsed.")
    }
}
