#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include <opencv2/core/utils/logger.hpp>

#include "interfaces/playerinfo.h"

int main()
{
	cv::utils::logging::setLogLevel(
		cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");

	asa::window::GetHandle(60, true);
	// asa::window::SetForeground();
	asa::settings::LoadGameUserSettings();
	asa::settings::LoadActionMappings();

	asa::resources::assetsDir = std::filesystem::path(
		"C:\\dev\\ASAPP\\ASAPP\\src\\assets");
	asa::resources::Init();

	asa::items::Init();

	auto info = asa::interfaces::PlayerInfo();



	while (true) {
		for (int i = 0; i < 6; i++) {
			std::cout << info.HasEquipped(nullptr,
							 static_cast<asa::interfaces::PlayerInfo::Slot>(i))
					  << std::endl;
		}
	}

	return 0;
}