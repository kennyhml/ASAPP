#pragma once
#include "game/globals.h"
#include "game/settings.h"
#include "game/window.h"



#include <iostream>


int main()
{

	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");
	asa::window::SetHandleTo("ArkAscended", 60, true);
	asa::settings::gameUserSettings::LoadGameUserSettings();

	return 0;
}