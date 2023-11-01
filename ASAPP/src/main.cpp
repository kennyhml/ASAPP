#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/settings.h"
#include "game/window.h"
#include <iostream>

#include "interfaces/interfacemanager.h"
int main()
{

	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");
	asa::window::SetHandleTo("ArkAscended", 60, true);
	asa::window::SetForeground();
	asa::settings::LoadGameUserSettings();



	asa::ASAInterfaceManager inter = asa::ASAInterfaceManager();

	while (true) {
		std::cout << inter.IsOpen() << std::endl;
	}

	return 0;
}