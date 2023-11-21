#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include "interfaces/localinventory.h"
#include "items/items.h"
int main()
{

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

	auto dedi = asa::interfaces::BaseInventory(true);

	dedi.searchBar.SearchFor("Hi hi hi");
	Sleep(300);
	dedi.searchBar.DeleteSearch();

	return 0;
}