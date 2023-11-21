#include "playerinfo.h"

using namespace asa::interfaces;

bool PlayerInfo::IsOpen()
{
	window::Color textColor(188, 244, 255);
	auto mask = window::GetMask(this->you.area, textColor, 20);
	return cv::countNonZero(mask) > 60;
}

const bool PlayerInfo::HasEquipped(const items::Item*, Slot slot)
{
	return false;
}

void PlayerInfo::UnequipItemAt(Slot slot) {}

const asa::items::Item* PlayerInfo::GetEquippedItem(Slot slot)
{
	return nullptr;
}
