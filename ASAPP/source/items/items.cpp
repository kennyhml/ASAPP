#include "asapp/items/items.h"
#include "../util/util.h"
#include "asapp/core/config.h"
#include <fstream>
#include <iostream>

namespace asa::items
{
	Item::Item(std::string name, cv::Mat icon)
		: Item(name, icon, rawData[name]){};

	Item::Item(std::string name, json itemData) : name(name)
	{
		icon_path = (core::config::assets_dir / itemData["icon"]).string();
		icon = cv::imread(icon_path, cv::IMREAD_UNCHANGED);
		if (icon.empty()) {
			std::cerr << "\t[!] Failure reading: '" << icon_path << "'"
					  << std::endl;
		}

		type = itemTypeMap[itemData["type"]];
		weight = itemData["weight"];
		stack_size = itemData["stack_size"];
		can_put_in_hotbar = itemData["can_put_in_hotbar"];
		has_spoil_timer = itemData["has_spoil_timer"];
		has_durability = itemData["has_durability"];
		requires_engram = itemData["requires_engram"];
		has_ambiguous_query = itemData["has_ambiguous_query"];
	}

	Item::Item(std::string name, cv::Mat icon, json itemData) : name(name)
	{
		icon = icon;
		type = itemTypeMap[itemData["type"]];
		weight = itemData["weight"];
		stack_size = itemData["stack_size"];
		can_put_in_hotbar = itemData["can_put_in_hotbar"];
		has_spoil_timer = itemData["has_spoil_timer"];
		has_durability = itemData["has_durability"];
		requires_engram = itemData["requires_engram"];
		has_ambiguous_query = itemData["has_ambiguous_query"];
	}

	const cv::Mat& Item::get_inventory_icon()
	{
		if (!inventory_icon.empty()) {
			return inventory_icon;
		}

		cv::resize(icon, rgba_inventory_icon,
			cv::Size(icon.cols * 0.24, icon.rows * 0.24), 0, 0,
			cv::INTER_LINEAR);

		cv::cvtColor(rgba_inventory_icon, inventory_icon, cv::COLOR_RGBA2RGB);
		return inventory_icon;
	}

	const cv::Mat& Item::get_inventory_icon_mask()
	{
		if (!inventory_icon_mask.empty()) {
			return inventory_icon_mask;
		}

		// Make sure the rgba inventory icon has been created already
		if (rgba_inventory_icon.empty()) {
			get_inventory_icon();
		}

		inventory_icon_mask = util::mask_alpha_channel(rgba_inventory_icon);
		return inventory_icon_mask;
	}

	const cv::Mat& Item::get_notification_icon()
	{
		if (!notification_icon.empty()) {
			return notification_icon;
		}
		cv::resize(icon, rgba_notification_icon,
			cv::Size(icon.cols * 0.11, icon.rows * 0.11), 0, 0,
			cv::INTER_LINEAR);

		cv::cvtColor(
			rgba_notification_icon, notification_icon, cv::COLOR_RGBA2RGB);
		return notification_icon;
	}

	const cv::Mat& Item::get_notification_icon_mask()
	{
		if (!notification_icon_mask.empty()) {
			return notification_icon_mask;
		}

		if (rgba_notification_icon.empty()) {
			get_notification_icon();
		}
		notification_icon_mask = util::mask_alpha_channel(
			rgba_notification_icon);
		return notification_icon_mask;
	}

	bool load_raw_data()
	{
		std::ifstream f(core::config::itemdata_path);
		if (!f.is_open()) {
			std::cout << "Failed to open itemdata.json!" << std::endl;
			return false;
		}

		rawData = json::parse(f);
		f.close();
		return true;
	}
	bool init()
	{
		auto start = std::chrono::system_clock::now();

		if (!load_raw_data()) {
			return false;
		}
		std::cout << "[+] Initializing predefined items..." << std::endl;
		if (!resources::init()) {
			return false;
		}

		auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now() - start);
		std::cout << "-> Loaded all assets within " << timeTaken << std::endl;
		return true;
	}

	void load(Item*& item, std::string name)
	{
		if (rawData.find(name) == rawData.end()) {
			std::cout << "[!] No item data found for: " << name << std::endl;
			return;
		}

		if (item) {
			delete item;
			item = nullptr;
		}

		item = new Item(name, rawData[name]);
		std::cout << std::format("\t[-] Loaded predef. item '{}'. Icon: '{}'",
						 item->name, item->icon_path)
				  << std::endl;
	}

	bool resources::init()
	{
		load(metal, "Metal");
		load(metal_ingot, "Metal Ingot");
		load(cementing_paste, "Cementing Paste");
		load(achatina_paste, "Achatina Paste");
		load(fiber, "Fiber");
		load(flint, "Flint");
		load(gunpowder, "Gunpowder");
		load(sparkpowder, "Sparkpowder");
		load(obsidian, "Obsidian");
		load(polymer, "Polymer");
		load(stone, "Stone");
		load(thatch, "Thatch");
		load(wood, "Wood");

		return true;
	}


}
