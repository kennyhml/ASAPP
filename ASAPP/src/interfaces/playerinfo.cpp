#include "playerinfo.h"
#include <stdexcept>

using namespace asa::interfaces;

bool PlayerInfo::IsOpen()
{
	window::Color textColor(188, 244, 255);
	auto mask = window::GetMask(this->you.area, textColor, 20);
	return cv::countNonZero(mask) > 60;
}

const bool PlayerInfo::HasEquipped(const items::Item* item, Slot slot)
{
	if (item && (!item->type == item->EQUIPPABLE)) {
		throw std::invalid_argument(
			std::format("Item '{}' cannot be equipped.", item->name));
	}

	GearSlot gearSlot = this->gearSlots[slot];
	const auto roi = item ? gearSlot : gearSlot.GetSlotDescriptionArea();

	if (!item) {
		auto mask = window::GetMask(roi, window::Color(223, 250, 255), 20);
		return cv::countNonZero(mask) < 30;
	}
}

void PlayerInfo::UnequipItemAt(Slot slot) {}

const asa::items::Item* PlayerInfo::GetEquippedItem(Slot slot)
{
	return nullptr;
}
