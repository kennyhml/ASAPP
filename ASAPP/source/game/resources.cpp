#include "asapp/game/resources.h"
#include "asapp/core/config.h"
#include <iostream>

#define LOAD_RESOURCE(dir, name)                                               \
	name = asa::resources::LoadResource(dir / (std::string(#name) + ".png"));

static bool IsValidAssetsDir(std::filesystem::path path)
{
	if (path.empty()) {
		std::cout << "[!] resources::Init requires resources::assetDir to be "
					 "specified!"
				  << std::endl;
		return false;
	}

	if (!std::filesystem::exists(path)) {
		std::cout << std::format(
						 "[!] Assets directory '{}' not found!", path.string())
				  << std::endl;
		return false;
	}
	return true;
}

cv::Mat asa::resources::LoadResource(std::filesystem::path path)
{
	auto mat = cv::imread(path.string());
	std::cout << "\t[-] Resource loaded @ " << path << std::endl;
	return mat;
}

bool asa::resources::Init()
{
	if (!IsValidAssetsDir(config::assetsDir)) {
		return false;
	}

	std::cout << "[+] Initializing resources..." << std::endl;
	if (!interfaces::InitInterfaces() || !text::InitText()) {
		return false;
	}

	return true;
}

bool asa::resources::interfaces::InitInterfaces()
{
	auto dir = config::assetsDir / "interfaces";
	if (!IsValidAssetsDir(dir)) {
		return false;
	}

	LOAD_RESOURCE(dir, cb_arrowdown);
	LOAD_RESOURCE(dir, regions);
	LOAD_RESOURCE(dir, lay_on);
	LOAD_RESOURCE(dir, day);
	LOAD_RESOURCE(dir, esc);
	LOAD_RESOURCE(dir, accept);
	LOAD_RESOURCE(dir, back);
	LOAD_RESOURCE(dir, join_last_session);
	LOAD_RESOURCE(dir, refresh);
	return true;
}

bool asa::resources::text::InitText()
{
	auto dir = config::assetsDir / "text";
	if (!IsValidAssetsDir(dir)) {
		return false;
	}
	LOAD_RESOURCE(dir, added);
	LOAD_RESOURCE(dir, removed);
	LOAD_RESOURCE(dir, day);
	LOAD_RESOURCE(dir, lootcrate);
	LOAD_RESOURCE(dir, default_teleport);
	LOAD_RESOURCE(dir, fast_travel);
	LOAD_RESOURCE(dir, access_inventory);
	LOAD_RESOURCE(dir, x);

	return true;
}