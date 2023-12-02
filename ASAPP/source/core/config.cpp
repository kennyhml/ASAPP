#include "asapp/core/config.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool asa::config::Init(std::filesystem::path configPath)
{
	std::ifstream f(configPath);
	if (!f.is_open()) {
		std::cerr << "[!] Failed to open config at " << configPath << std::endl;
		return false;
	}
	json data = json::parse(f);
	f.close();

	bool environmentSet = (SetGameDirectory(data["gameBaseDirectory"]) &&
						   SetAssetsDirectory(data["assetsDir"]) &&
						   SetItemdataPath(data["itemData"]) &&
						   SetTessdataPath(data["tessdataPath"]));

	if (!environmentSet) {
		std::cerr << "[!] One or more environment variables is faulty. ASAPP "
					 "was not initialized successfully."
				  << std::endl;
		return false;
	}
	std::cout << "[+] Environment variables set successfully." << std::endl;
	return true;
}


bool asa::config::SetGameDirectory(std::filesystem::path path)
{
	if (!std::filesystem::exists(path)) {
		std::cerr << "[!] Failed to set game directory. " << path
				  << " does not exist." << std::endl;
	}
	gameBaseDirectory = path;
	return true;
}

bool asa::config::SetAssetsDirectory(std::filesystem::path path)
{
	if (!std::filesystem::exists(path)) {
		std::cerr << "[!] Failed to set assets directory. " << path
				  << " does not exist." << std::endl;
	}
	assetsDir = path;
	return true;
}

bool asa::config::SetItemdataPath(std::filesystem::path path)
{
	if (!std::filesystem::exists(path)) {
		std::cerr << "[!] Failed to set itemdata.json path. " << path
				  << " does not exist." << std::endl;
	}
	itemdataPath = path;
	return true;
}

bool asa::config::SetTessdataPath(std::filesystem::path path)
{
	if (!std::filesystem::exists(path)) {
		std::cerr << "[!] Failed to set Tesseract data path. " << path
				  << " does not exist." << std::endl;
	}
	tessdataPath = path;
	return true;
}