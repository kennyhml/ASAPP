#pragma once
#include "items.h"
#include <unordered_map>

struct Structure;
struct Item;

namespace asa::items
{
	struct Recipe
	{
		Structure* craftIn = nullptr;
		std::unordered_map<Item*, uint16_t> requiredMaterials;
	};
}