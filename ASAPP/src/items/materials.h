#pragma once
#include "item.h"

namespace asa::items::materials
{
	bool InitMaterials();
	void LoadMat(Item*& item, std::string name, cv::Mat icon, Recipe* recipe,
		float weight, int stackSize, bool ambigious = false,
		bool hotbarable = false, bool spoils = false);

	inline Item* metal = nullptr;
	inline Item* metalIngot = nullptr;
	inline Item* cementingPaste = nullptr;
	inline Item* fiber = nullptr;
	inline Item* flint = nullptr;
	inline Item* gunpowder = nullptr;
	inline Item* sparkpowder = nullptr;
	inline Item* obsidian = nullptr;
	inline Item* polymer = nullptr;
	inline Item* stone = nullptr;
	inline Item* thatch = nullptr;
	inline Item* wood = nullptr;
}
