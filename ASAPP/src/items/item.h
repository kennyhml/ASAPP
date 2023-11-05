#pragma once
#include "../game/resources.h"
#include "recipe.h"
#include <filesystem>
#include <string>

struct Recipe;

namespace asa::items
{
	struct Item
	{
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

		bool canBePlacedInHotbar;
		bool hasSpoilTimer;
		bool hasDurability;
		bool requiresEngram;
	};
}
