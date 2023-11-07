#include "materials.h"
#include "../game/resources.h"

bool asa::items::materials::InitMaterials()
{
	LoadMat(metal, "Metal", asa::resources::metal, nullptr, 1.0f, 300, true);
	LoadMat(metalIngot, "Metal Ingot", asa::resources::metal_ingot, nullptr,
		1.0f, 300);
	LoadMat(cementingPaste, "Cementing Paste", asa::resources::cementing_paste,
		nullptr, 0.01f, 100);
	LoadMat(fiber, "Fiber", asa::resources::fiber, nullptr, 0.01f, 300);
	LoadMat(flint, "Flint", asa::resources::flint, nullptr, 0.05f, 100);
	LoadMat(
		gunpowder, "Gunpowder", asa::resources::gunpowder, nullptr, 0.1f, 100);
	LoadMat(obsidian, "Obsidian", asa::resources::obsidian, nullptr, 1.f, 100);
	LoadMat(
		polymer, "Polymer", asa::resources::polymer, nullptr, 0.25f, 100, true);
	LoadMat(stone, "Stone", asa::resources::stone, nullptr, 0.5f, 100, true);
	LoadMat(thatch, "Thatch", asa::resources::thatch, nullptr, 0.02f, 100),
		true;
	LoadMat(wood, "Wood", asa::resources::wood, nullptr, 0.5f, 100, true);
	LoadMat(sparkpowder, "Sparkpowder", asa::resources::sparkpowder, nullptr,
		0.1f, 100);

	return true;
}

void asa::items::materials::LoadMat(Item*& item, std::string name, cv::Mat icon,
	Recipe* recipe, float weight, int stackSize, bool ambigious,
	bool hotbarable, bool spoils)
{
	if (item) {
		delete item;
	}

	item = new Item{ name, icon, Item::RESOURCE, recipe, weight, stackSize,
		hotbarable, spoils, false, false, ambigious };
}