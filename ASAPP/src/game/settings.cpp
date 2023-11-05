#include "settings.h"
#include "globals.h"
#include <format>
#include <fstream>
#include <sstream>

using namespace asa;

#define VERBOSE_LOG(log)                                                       \
	if (verbose) {                                                             \
		std::cout << log << std::endl;                                         \
	}

bool settings::OpenFile(
	bool verbose, std::filesystem::path path, std::ifstream& fileOut)
{
	if (!std::filesystem::exists(path)) {
		std::cout << std::format("[!] Path '{}' was not found.", path.string());
		return false;
	}

	fileOut.open(path.c_str());
	if (!fileOut.is_open()) {
		std::cout << std::format("[!] Failed to open '{}'", path.string())
				  << std::endl;
		return false;
	}
}

std::ostream& settings::actionMappings::operator<<(
	std::ostream& os, const settings::actionMappings::ActionMapping& m)
{
	return os << std::format("ActionMapping(name={}, shift={}, "
							 "ctrl={}, alt={}, cmd={}, key={})",
			   m.name, int(m.shift), int(m.ctrl), int(m.alt), int(m.cmd),
			   m.key);
}

static bool ParseKeyValue(
	std::string token, std::string& keyOut, std::string& valueOut)
{
	size_t eq = token.find('=');
	if (eq == std::string::npos) {
		return false;
	}

	keyOut = token.substr(0, eq);
	// Parse out the value, example: ActionName="AccessInventory"
	// Exclude  = and " characters from the key  ---------------
	if (keyOut == "ActionName") {
		valueOut = token.substr(eq + 2, token.length() - (keyOut.length() + 3));
	}
	// Parse out the ending parantheses, example: Key=F)
	else if (keyOut == "Key") {
		valueOut = token.substr(eq + 1, token.length() - keyOut.length() - 2);
	}
	else {
		valueOut = token.substr(eq + 1);
	}
	return true;
}

static std::any ConvertSettingValue(std::string key, std::string value)
{
	if (key == "LastJoinedSessionPerCategory") {
		return value;
	}
	else if (value.find(".") != std::string::npos) {
		return std::stof(value);
	}
	else if (value == "True" || value == "False") {
		return value == "True";
	}
	else {
		return std::stoi(value);
	}
}

static bool ParseUserSetting(std::istringstream& stream, bool verbose)
{
	auto& map = settings::gameUserSettings::settingValueMap;

	std::string key;
	std::string value;

	if (!ParseKeyValue(stream.str(), key, value)) {
		return false;
	}

	if (map.find(key) == map.end()) {
		return false;
	}

	map[key] = ConvertSettingValue(key, value);
	VERBOSE_LOG("\t[-] Parsed " << key << " (" << value << ")");
	return true;
}

static bool ParseActionMapping(std::istringstream& stream, bool verbose)
{
	auto& map = settings::actionMappings::inputMap;

	// skip the first 16 characters 'ActionMappings=('
	stream.seekg(16);

	std::string token;
	std::vector<std::string> tokens;

	while (std::getline(stream, token, ',')) {
		tokens.push_back(token);
	}

	settings::actionMappings::ActionMapping* mapping = nullptr;

	for (const std::string& token : tokens) {
		std::string key;
		std::string value;
		if (!ParseKeyValue(token, key, value)) {
			continue;
		}

		if (key == "ActionName") {
			if (map.find(value) == map.end()) {
				return false; // we only care for some action mappings
			}
			mapping = map[value];
			continue;
		}

		if (!mapping) {
			std::cout << std::format("[!] Unable to parse '{}'", stream.str())
					  << std::endl;
			return false;
		}

		if (key == "bShift") {
			mapping->shift = (value == "True");
		}
		else if (key == "bCtrl") {
			mapping->ctrl = (value == "True");
		}
		else if (key == "bAlt") {
			mapping->alt = (value == "True");
		}
		else if (key == "bCmd") {
			mapping->cmd = (value == "True");
		}
		else if (key == "Key") {
			mapping->key = value;
		}
	}
	VERBOSE_LOG("\t[-] Parsed " << *mapping)
	return true;
}

bool settings::actionMappings::LoadActionMappings(bool verbose)
{
	std::ifstream file;
	if (!OpenFile(verbose, globals::gameBaseDirectory / inputsRelPath, file)) {
		return false;
	}

	VERBOSE_LOG("[+] Parsing Input.ini...");
	for (std::string line; std::getline(file, line);) {
		if (line.find("ActionMappings=") != std::string::npos) {
			std::istringstream ss(line);
			ParseActionMapping(ss, verbose);
		}
	}
	VERBOSE_LOG("[+] Input.ini parsed, mappings mapped.")
}

bool settings::gameUserSettings::LoadGameUserSettings(bool verbose)
{
	std::ifstream file;
	auto path = globals::gameBaseDirectory / userSettingsRelPath;

	if (!OpenFile(verbose, path, file)) {
		return false;
	}

	VERBOSE_LOG("[+] Parsing GameUserSettings.ini...")
	bool sectionFound = false;

	for (std::string line; std::getline(file, line);) {

		bool sectionStart = line.find("ShooterGame") != std::string::npos;
		if (line.find("ServerSettings") != std::string::npos) {
			break;
		}

		if (!sectionFound && !sectionStart) {
			continue;
		}

		if (sectionStart) {
			sectionFound = true;
			continue;
		}

		std::istringstream ss(line);
		ParseUserSetting(ss, verbose);
	}
	VERBOSE_LOG("[+] GameUserSettings.ini parsed.")
}