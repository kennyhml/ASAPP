#include "items.h"
#include <fstream>
#include <iostream>

asa::items::Item::Item(std::string name, cv::Mat icon)
	: Item(name, icon, rawData[name]){};

asa::items::Item::Item(std::string name, json itemData) : name(name)
{
	this->icon = cv::imread(
		(asa::resources::assetsDir / itemData["icon"]).string());

	this->type = itemTypeMap[itemData["type"]];
	this->weight = itemData["weight"];
	this->stackSize = itemData["stackSize"];
	this->canPutInHotbar = itemData["canPutInHotbar"];
	this->hasSpoilTimer = itemData["hasSpoilTimer"];
	this->hasDurability = itemData["hasDurability"];
	this->requiresEngram = itemData["requiresEngram"];
	this->hasAmbigiousQuery = itemData["hasAmbigiousQuery"];
}

asa::items::Item::Item(std::string name, cv::Mat icon, json itemData)
	: name(name)
{
	this->icon = icon;
	this->type = itemTypeMap[itemData["type"]];
	this->weight = itemData["weight"];
	this->stackSize = itemData["stackSize"];
	this->canPutInHotbar = itemData["canPutInHotbar"];
	this->hasSpoilTimer = itemData["hasSpoilTimer"];
	this->hasDurability = itemData["hasDurability"];
	this->requiresEngram = itemData["requiresEngram"];
	this->hasAmbigiousQuery = itemData["hasAmbigiousQuery"];
}

bool asa::items::LoadRawData()
{
	std::ifstream f("src\\itemdata.json");
	if (!f.is_open()) {
		std::cout << "Failed to open itemdata.json!" << std::endl;
		return false;
	}

	rawData = json::parse(f);
	f.close();
	return true;
}
bool asa::items::Init()
{
	if (!LoadRawData()) {
		return false;
	}

	std::cout << "[+] Initializing predefined items..." << std::endl;

	if (!resources::Init() || !equippables::Init()) {
		return false;
	}
	return true;
}

void asa::items::Load(Item*& item, std::string name, cv::Mat icon)
{
	if (item) {
		delete item;
		item = nullptr;
	}
	item = new Item(name, icon);
	std::cout << "\t[-] Predefined item loaded: " << item->name << std::endl;
}

bool asa::items::resources::Init()
{
	Load(metal, "Metal", asa::resources::metal);
	Load(metalIngot, "Metal Ingot", asa::resources::metal_ingot);
	Load(cementingPaste, "Cementing Paste", asa::resources::paste);
	Load(achatinaPaste, "Achatina Paste", asa::resources::paste);
	Load(fiber, "Fiber", asa::resources::fiber);
	Load(flint, "Flint", asa::resources::flint);
	Load(gunpowder, "Gunpowder", asa::resources::gunpowder);
	Load(sparkpowder, "Sparkpowder", asa::resources::sparkpowder);
	Load(obsidian, "Obsidian", asa::resources::obsidian);
	Load(polymer, "Polymer", asa::resources::polymer);
	Load(stone, "Stone", asa::resources::stone);
	Load(thatch, "Thatch", asa::resources::thatch);
	Load(wood, "Wood", asa::resources::wood);

	return true;
}


bool asa::items::equippables::Init()
{
	Load(gasmask, "Gas Mask", asa::resources::gasmask);
	return true;
}