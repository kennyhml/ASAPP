#pragma once
#include "asapp/game/resources.h"
#include "recipe.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace asa::items
{
	struct Item
	{
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
		cv::Mat icon;
		ItemType type;
		Recipe* recipe = nullptr;

		float weight;
		int stackSize;

		bool canPutInHotbar;
		bool hasSpoilTimer;
		bool hasDurability;
		bool requiresEngram;
		bool hasAmbigiousQuery;
	};

	inline std::unordered_map<std::string, Item::ItemType> itemTypeMap = {
		{ "CONSUMABLE", Item::CONSUMABLE }, { "EQUIPPABLE", Item::EQUIPPABLE },
		{ "WEAPON", Item::WEAPON }, { "AMMO", Item::AMMO },
		{ "STRUCTURE", Item::STRUCTURE }, { "RESOURCE", Item::RESOURCE },
		{ "ATTACHMENT", Item::ATTACHMENT }, { "ARTIFACT", Item::ARTIFACT }
	};



}
