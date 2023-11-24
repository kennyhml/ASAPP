#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include "interfaces/travelmap.h"
#include <opencv2/core/utils/logger.hpp>

#include "entities/dinoent.h"
#include "entities/localplayer.h"
#include "structures/container.h"
#include <sstream>

void EmptySnail()
{
	auto snail = asa::entities::DinoEnt("Achatina");

	asa::entities::gLocalPlayer->Access(&snail);
	if (snail.inventory->slots[0].HasItem(
			asa::items::resources::achatinaPaste)) {
		snail.inventory->TakeSlot(0);
	}
	snail.Exit();
}

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


	auto vault = asa::structures::Container("Vault");

	std::cout << "..." << std::endl;


	for (int i = 1; i < 7; i++) {

		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << i;
		std::string bed = "PASTE" + oss.str();

		asa::interfaces::gTravelMap->GoTo(bed);
		Sleep(5000);

		asa::entities::gLocalPlayer->Crouch();
		asa::entities::gLocalPlayer->TurnDown(12);
		Sleep(200);

		EmptySnail();

		asa::entities::gLocalPlayer->TurnRight(35);
		Sleep(200);
		EmptySnail();

		asa::entities::gLocalPlayer->TurnLeft(70);
		Sleep(200);
		EmptySnail();
		Sleep(200);

		asa::entities::gLocalPlayer->TurnLeft(55);
		asa::entities::gLocalPlayer->TurnLeft();
		Sleep(1000);

		while (!asa::entities::gLocalPlayer->DepositedItem()) {
			asa::window::Press(asa::settings::use);
			Sleep(100);
		}

		asa::entities::gLocalPlayer->TurnDown(50);
		Sleep(400);
		asa::controls::Press(asa::settings::use);
		Sleep(3000);
	}
}
