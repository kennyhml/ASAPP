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

	asa::window::SetHandleTo("ArkAscended", 60, true);
	asa::window::SetForeground();
	asa::settings::LoadGameUserSettings();
	asa::settings::LoadActionMappings();

	asa::resources::assetsDir = std::filesystem::path(
		"C:\\dev\\ASAPP\\ASAPP\\src\\assets");
	asa::resources::Init();

	asa::items::Init();
	Sleep(2000);


	asa::controls::Press(asa::settings::fire);


	return 0;
}