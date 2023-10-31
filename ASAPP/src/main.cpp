#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/settings.h"
#include "game/window.h"
#include <iostream>


int main()
{

	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");
	asa::window::SetHandleTo("ArkAscended", 60, true);

	asa::settings::LoadGameUserSettings();
	Sleep(2000);

	for (int i = 0; i < 4; i++) {
		Sleep(200);
		asa::controls::TurnDegrees(90, 0);
	}
	return 0;
}