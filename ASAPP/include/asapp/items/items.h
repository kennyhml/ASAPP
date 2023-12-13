#pragma once
#include "item.h"
#include <filesystem>

struct Recipe;
using json = nlohmann::json;

namespace asa::items
{
	inline json raw_data = NULL;

	bool init();
	bool load_raw_data();

	Item* get_custom_item(std::string itemName);
	void load(Item*& item, std::string name);

	namespace resources
	{
		bool init();

		inline Item* metal = nullptr;
		inline Item* metal_ingot = nullptr;
		inline Item* cementing_paste = nullptr;
		inline Item* achatina_paste = nullptr;
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
