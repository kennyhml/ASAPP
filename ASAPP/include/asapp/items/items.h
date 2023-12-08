#pragma once
#include "item.h"
#include <filesystem>

struct Recipe;
using json = nlohmann::json;

namespace asa::items
{
	inline json rawData = NULL;

	bool Init();
	bool LoadRawData();

	Item* GetCustomItem(std::string itemName);
	void Load(Item*& item, std::string name);

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

}
