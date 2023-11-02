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

	auto inv = asa::interfaces::BaseInventory(false);

	inv.searchBar.Press();
	inv.searchBar.SearchFor("Meow");

	Sleep(2000);
	inv.searchBar.DeleteSearch();

	return 0;
}