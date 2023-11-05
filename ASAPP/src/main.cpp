#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include "interfaces/exceptions.h"
#include "interfaces/localinventory.h"
#include <iostream>
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


	auto inv = asa::interfaces::LocalInventory();

	try {
		throw asa::interfaces::exceptions::InterfaceNotOpenedError(&inv);
	}
	catch (asa::interfaces::exceptions::InterfaceError& e) {

		std::cout << e.what() << std::endl;
	}



	inv.Open();

	return 0;
}