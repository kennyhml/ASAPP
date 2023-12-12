#include "asapp/core/config.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace asa::core::config
{
	bool set_environment(std::filesystem::path config_path)
	{
		std::ifstream f(config_path);
		if (!f.is_open()) {
			std::cerr << "[!] Failed to open config at " << config_path
					  << std::endl;
			return false;
		}
		json data = json::parse(f);
		f.close();

		bool environment_set = (set_game_directory(data["gameBaseDirectory"]) &&
								set_assets_directory(data["assetsDir"]) &&
								set_itemdata_path(data["itemData"]) &&
								set_tessdata_path(data["tessdataPath"]));
		if (!environment_set) {
			std::cerr
				<< "[!] One or more environment variables is faulty. ASAPP "
				   "was not initialized successfully."
				<< std::endl;
			return false;
		}
		std::cout << "[+] Environment variables set successfully." << std::endl;
		return true;
	}

	bool set_game_directory(std::filesystem::path path)
	{
		if (!std::filesystem::exists(path)) {
			std::cerr << "[!] Failed to set game directory. " << path
					  << " does not exist." << std::endl;
		}
		game_base_directory = path;
		return true;
	}

	bool set_assets_directory(std::filesystem::path path)
	{
		if (!std::filesystem::exists(path)) {
			std::cerr << "[!] Failed to set assets directory. " << path
					  << " does not exist." << std::endl;
		}
		assets_dir = path;
		return true;
	}

	bool set_itemdata_path(std::filesystem::path path)
	{
		if (!std::filesystem::exists(path)) {
			std::cerr << "[!] Failed to set itemdata.json path. " << path
					  << " does not exist." << std::endl;
		}
		itemdata_path = path;
		return true;
	}

	bool set_tessdata_path(std::filesystem::path path)
	{
		if (!std::filesystem::exists(path)) {
			std::cerr << "[!] Failed to set Tesseract data path. " << path
					  << " does not exist." << std::endl;
		}
		tessdata_path = path;
		return true;
	}
}
