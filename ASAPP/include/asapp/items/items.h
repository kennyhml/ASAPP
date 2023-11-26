#pragma once
#include "asapp/game/resources.h"
#include "recipe.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

struct Recipe;
using json = nlohmann::json;

namespace asa::items
{
	bool Init();
	bool LoadRawData();

	inline json rawData = NULL;

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

	Item* GetCustomItem(std::string itemName);
	void Load(Item*& item, std::string name, cv::Mat icon);

	namespace resources
	{
		bool Init();

		inline Item* metal = nullptr;
		inline Item* metalIngot = nullptr;
		inline Item* cementingPaste = nullptr;
		inline Item* achatinaPaste = nullptr;
		inline Item* fiber = nullptr;
		inline Item* flint = nullptr;
		inline Item* gunpowder = nullptr;
		inline Item* sparkpowder = nullptr;
		inline Item* obsidian = nullptr;
		inline Item* polymer = nullptr;
		inline Item* stone = nullptr;
		inline Item* thatch = nullptr;
		inline Item* wood = nullptr;
		inline Item* gasmask = nullptr;

	}

	namespace equippables
	{
		bool Init();
		inline Item* gasmask = nullptr;
	}

}
