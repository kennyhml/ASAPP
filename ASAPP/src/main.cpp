#pragma once
#include "game/controls.h"
#include "game/globals.h"
#include "game/resources.h"
#include "game/settings.h"
#include "game/window.h"
#include "interfaces/localinventory.h"
#include "items/materials.h"

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

	asa::items::materials::InitMaterials();


	auto inv = asa::interfaces::LocalInventory();
	auto dedi = asa::interfaces::BaseInventory(true);


	while (!GetAsyncKeyState(VK_DELETE) & 0x1) {
		asa::controls::KeyPress("t");
		Sleep(3000);
	}

	return 0;
}