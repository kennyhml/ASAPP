#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/settings.h"
#include "game/window.h"
#include <iostream>

#include "interfaces/baseinventory.h"
#include "interfaces/interfacemanager.h"

int main()
{
	asa::globals::gameBaseDirectory = std::filesystem::path(
		"F:\\SteamLibrary\\steamapps\\common\\ARK Survival Ascended");
	asa::window::SetHandleTo("ArkAscended", 60, true);
	asa::window::SetForeground();
	asa::settings::LoadGameUserSettings();

	auto inv = asa::interfaces::BaseInventory(true);

	while (true) {

		std::cout << inv.folderViewButton.IsToggled() << std::endl;
	}

	return 0;
}