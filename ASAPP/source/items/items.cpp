#include "asapp/items/items.h"
#include "../util/util.h"
#include "asapp/core/config.h"
#include <fstream>
#include <iostream>

asa::items::Item::Item(std::string name, cv::Mat icon)
	: Item(name, icon, rawData[name]){};

asa::items::Item::Item(std::string name, json itemData) : name(name)
{
	iconPath = (config::assetsDir / itemData["icon"]).string();

	// items are loaded rgba so that we can drop the alpha channel to create a
	// mask scale them down by 0.24x for inventory icons and 0.11x for added /
	// removed.
	icon = cv::imread(iconPath, cv::IMREAD_UNCHANGED);
	if (icon.empty()) {
		std::cerr << "\t[!] Failure reading: '" << iconPath << "'" << std::endl;
	}

	type = itemTypeMap[itemData["type"]];
	weight = itemData["weight"];
	stackSize = itemData["stackSize"];
	canPutInHotbar = itemData["canPutInHotbar"];
	hasSpoilTimer = itemData["hasSpoilTimer"];
	hasDurability = itemData["hasDurability"];
	requiresEngram = itemData["requiresEngram"];
	hasAmbigiousQuery = itemData["hasAmbigiousQuery"];
}

asa::items::Item::Item(std::string name, cv::Mat icon, json itemData)
	: name(name)
{
	icon = icon;
	type = itemTypeMap[itemData["type"]];
	weight = itemData["weight"];
	stackSize = itemData["stackSize"];
	canPutInHotbar = itemData["canPutInHotbar"];
	hasSpoilTimer = itemData["hasSpoilTimer"];
	hasDurability = itemData["hasDurability"];
	requiresEngram = itemData["requiresEngram"];
	hasAmbigiousQuery = itemData["hasAmbigiousQuery"];
}

const cv::Mat& asa::items::Item::GetInventoryIcon()
{
	if (!inventoryIcon.empty()) {
		return inventoryIcon;
	}

	cv::resize(icon, rgbaInventoryIcon,
		cv::Size(icon.cols * 0.24, icon.rows * 0.24), 0, 0, cv::INTER_LINEAR);

	cv::cvtColor(rgbaInventoryIcon, inventoryIcon, cv::COLOR_RGBA2RGB);
	return inventoryIcon;
}

const cv::Mat& asa::items::Item::GetInventoryIconMask()
{
	if (!inventoryIconMask.empty()) {
		return inventoryIconMask;
	}

	// Make sure the rgba inventory icon has been created already
	if (rgbaInventoryIcon.empty()) {
		GetInventoryIcon();
	}

	inventoryIconMask = util::MaskAlphaChannel(rgbaInventoryIcon);
	return inventoryIconMask;
}

const cv::Mat& asa::items::Item::GetNotificationIcon()
{
	if (!notificationIcon.empty()) {
		return notificationIcon;
	}
	cv::resize(icon, rgbaNotificationIcon,
		cv::Size(icon.cols * 0.11, icon.rows * 0.11), 0, 0, cv::INTER_LINEAR);

	cv::cvtColor(rgbaNotificationIcon, notificationIcon, cv::COLOR_RGBA2RGB);
	return notificationIcon;
}

const cv::Mat& asa::items::Item::GetNotificationMask()
{
	if (!notificationIconMask.empty()) {
		return notificationIconMask;
	}

	if (rgbaNotificationIcon.empty()) {
		GetNotificationIcon();
	}
	notificationIconMask = util::MaskAlphaChannel(rgbaNotificationIcon);
	return notificationIconMask;
}

bool asa::items::LoadRawData()
{
	std::ifstream f(config::itemdataPath);
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
	auto start = std::chrono::system_clock::now();

	if (!LoadRawData()) {
		return false;
	}
	std::cout << "[+] Initializing predefined items..." << std::endl;
	if (!resources::Init()) {
		return false;
	}

	auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - start);
	std::cout << "-> Loaded all assets within " << timeTaken << std::endl;
	return true;
}

void asa::items::Load(Item*& item, std::string name)
{
	if (rawData.find(name) == rawData.end()) {
		std::cout << "[!] No item data found for: " << name << std::endl;
		return;
	}

	if (item) {
		delete item;
		item = nullptr;
	}

	item = new Item(name, asa::items::rawData[name]);
	std::cout << std::format("\t[-] Loaded predef. item '{}'. Icon: '{}'",
					 item->name, item->iconPath)
			  << std::endl;
}

bool asa::items::resources::Init()
{
	Load(metal, "Metal");
	Load(metalIngot, "Metal Ingot");
	Load(cementingPaste, "Cementing Paste");
	Load(achatinaPaste, "Achatina Paste");
	Load(fiber, "Fiber");
	Load(flint, "Flint");
	Load(gunpowder, "Gunpowder");
	Load(sparkpowder, "Sparkpowder");
	Load(obsidian, "Obsidian");
	Load(polymer, "Polymer");
	Load(stone, "Stone");
	Load(thatch, "Thatch");
	Load(wood, "Wood");

	return true;
}