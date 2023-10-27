#pragma once
#include "recipe.h"
#include <string>

struct Recipe;

namespace asa
{
	struct IASAItem
	{
		std::string name;
		Recipe* recipe;
	};
}
