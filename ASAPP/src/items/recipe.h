#pragma once
#include "iasaitem.h"
#include <unordered_map>

struct Structure;
struct IASAItem;

namespace asa
{
	struct Recipe
	{
		Structure* craftIn = nullptr;
		std::unordered_map<IASAItem*, uint16_t> requiredMaterials;
	};
}