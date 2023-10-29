#include "settings.h"
#include "globals.h"
#include <format>
#include <fstream>
#include <sstream>

using namespace asa;

std::ostream& settings::actionMappings::operator<<(
	std::ostream& os, const settings::actionMappings::ActionMapping& m)
{
	return os << std::format("ActionMapping(name={}, shift={}, "
							 "ctrl={}, alt={}, cmd={}, key={})",
			   m.name, int(m.shift), int(m.ctrl), int(m.alt), int(m.cmd),
			   m.key);
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
		size_t eq = token.find('=');
		if (eq == std::string::npos) {
			continue;
		}

		std::string key = token.substr(0, eq);

		std::string value;

		// Parse out the value, example: ActionName="AccessInventory"
		// Exclude  = and " characters from the key  ---------------
		if (key == "ActionName") {
			value = token.substr(eq + 2, token.length() - (key.length() + 3));
		}
		// Parse out the ending parantheses, example: Key=F)
		else if (key == "Key") {
			value = token.substr(eq + 1, token.length() - key.length() - 2);
		}
		else {
			value = token.substr(eq + 1);
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
	if (verbose) {
		std::cout << "\t[-] Parsed " << *mapping << std::endl;
	}
	return true;
}

bool settings::actionMappings::LoadActionMappings(bool verbose)
{
	auto path = globals::gameBaseDirectory / inputsRelPath;
	if (!std::filesystem::exists(path)) {
		std::cout << std::format("[!] Path '{}' was not found.", path.string());
		return false;
	}

	std::ifstream inFile(path.c_str());
	if (!inFile.is_open()) {
		std::cout << std::format("[!] Failed to open '{}'", path.string())
				  << std::endl;
		return false;
	}

	if (verbose) {
		std::cout << "[+] Parsing Input.ini..." << std::endl;
	}

	for (std::string line; std::getline(inFile, line);) {

		// we only care about the action mappings
		if (line.find("ActionMappings=") == std::string::npos) {
			continue;
		}

		std::istringstream ss(line);
		ParseActionMapping(ss, verbose);
	}

	if (verbose) {
		std::cout << "[+] Input.ini parsed, mappings mapped." << std::endl;
	}
}