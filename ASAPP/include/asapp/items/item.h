#pragma once
#include "asapp/game/resources.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace asa::items
{
	struct Item
	{
	public:
		Item(std::string name, json itemData);
		Item(std::string name, cv::Mat icon);
		Item(std::string name, cv::Mat icon, json itemData);

		enum ItemType
		{
			CONSUMABLE,
			EQUIPPABLE,
			WEAPON,
			AMMO,
			STRUCTURE,
			RESOURCE,
			ATTACHMENT,
			ARTIFACT,
		};

		std::string name;
		std::string icon_path;
		cv::Mat icon;
		ItemType type;

		float weight;
		int stackSize;

		bool can_put_in_hotbar;
		bool has_spoil_timer;
		bool has_durability;
		bool requires_engram;
		bool has_ambigious_query;

		const cv::Mat& GetInventoryIcon();
		const cv::Mat& GetInventoryIconMask();
		const cv::Mat& GetNotificationIcon();
		const cv::Mat& GetNotificationMask();

	private:
		cv::Mat rgbaInventoryIcon;
		cv::Mat rgbaNotificationIcon;

		cv::Mat inventoryIcon;
		cv::Mat inventoryIconMask;
		cv::Mat notificationIcon;
		cv::Mat notificationIconMask;
	};

	inline std::unordered_map<std::string, Item::ItemType> itemTypeMap = {
		{ "CONSUMABLE", Item::CONSUMABLE }, { "EQUIPPABLE", Item::EQUIPPABLE },
		{ "WEAPON", Item::WEAPON }, { "AMMO", Item::AMMO },
		{ "STRUCTURE", Item::STRUCTURE }, { "RESOURCE", Item::RESOURCE },
		{ "ATTACHMENT", Item::ATTACHMENT }, { "ARTIFACT", Item::ARTIFACT }
	};



}
