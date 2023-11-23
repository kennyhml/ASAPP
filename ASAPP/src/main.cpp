#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include "interfaces/travelmap.h"
#include <opencv2/core/utils/logger.hpp>

#include "entities/localplayer.h"

int main()
{
	cv::utils::logging::setLogLevel(
		cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");

	asa::window::GetHandle(60, true);
	asa::window::SetForeground();
	asa::settings::LoadGameUserSettings();
	asa::settings::LoadActionMappings();

	asa::resources::assetsDir = std::filesystem::path(
		"C:\\dev\\ASAPP\\ASAPP\\src\\assets");
	asa::resources::Init();
	asa::items::Init();



	asa::interfaces::gTravelMap->GoTo("PASTE03");
	Sleep(5000);

	asa::entities::gLocalPlayer->Crouch();


	auto snailInv = asa::interfaces::BaseInventory(true);
	asa::entities::gLocalPlayer->TurnDown(12);
	Sleep(2000);

	asa::entities::gLocalPlayer->TurnRight(35);
	Sleep(2000);
	asa::entities::gLocalPlayer->TurnLeft(70);
}
