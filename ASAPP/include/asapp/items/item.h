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
		int stack_size;

		bool can_put_in_hotbar;
		bool has_spoil_timer;
		bool has_durability;
		bool requires_engram;
		bool has_ambiguous_query;

		const cv::Mat& get_inventory_icon();
		const cv::Mat& get_inventory_icon_mask();
		const cv::Mat& get_notification_icon();
		const cv::Mat& get_notification_icon_mask();

	private:
		cv::Mat rgba_inventory_icon;
		cv::Mat rgba_notification_icon;

		cv::Mat inventory_icon;
		cv::Mat inventory_icon_mask;
		cv::Mat notification_icon;
		cv::Mat notification_icon_mask;
	};

	inline std::unordered_map<std::string, Item::ItemType> itemTypeMap = {
		{ "CONSUMABLE", Item::CONSUMABLE }, { "EQUIPPABLE", Item::EQUIPPABLE },
		{ "WEAPON", Item::WEAPON }, { "AMMO", Item::AMMO },
		{ "STRUCTURE", Item::STRUCTURE }, { "RESOURCE", Item::RESOURCE },
		{ "ATTACHMENT", Item::ATTACHMENT }, { "ARTIFACT", Item::ARTIFACT }
	};



}
