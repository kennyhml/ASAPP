#include "resources.h"
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
	std::cout << "\t[-] Resource loaded -> " << path << std::endl;
	return mat;
}

bool asa::resources::Init()
{
	if (!IsValidAssetsDir(assetsDir)) {
		return false;
	}

	std::cout << "[+] Initializing resources..." << std::endl;

	if (!InitInterfaces()) {
		return false;
	}
	return true;
}

bool asa::resources::interfaces::InitInterfaces()
{
	auto dir = assetsDir / "interfaces";
	if (!IsValidAssetsDir(dir)) {
		return false;
	}

	LOAD_RESOURCE(dir, cb_arrowdown);
}
