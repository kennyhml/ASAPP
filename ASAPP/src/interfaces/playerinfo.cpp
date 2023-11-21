#include "playerinfo.h"
#include "../game/globals.h"
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

void PlayerInfo::UnequipItemAt(Slot slot)
{
	const auto& gearSlot = this->gearSlots[slot];
	while (this->HasEquipped(nullptr, slot)) {
		auto point = gearSlot.GetRandLocation(5);
		if (globals::useWindowInput) {
			window::ClickAt(point, controls::LEFT);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			window::ClickAt(point, controls::LEFT);
		}
		else {
			window::SetMousePos(point);
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			for (int i = 0; i < 3; i++) {
				controls::Press(settings::use);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void PlayerInfo::UnequipAllItems()
{
	bool anyLeft = true;
	while (anyLeft) {
		anyLeft = false;

		for (const auto& slot : gearSlots) {

			auto point = slot.GetRandLocation(5);
			if (globals::useWindowInput) {
				window::ClickAt(slot.GetRandLocation(5), controls::LEFT);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				window::ClickAt(slot.GetRandLocation(5), controls::LEFT);
			}
			else {
				window::SetMousePos(point);
				std::this_thread::sleep_for(std::chrono::milliseconds(15));
				for (int i = 0; i < 3; i++) {
					controls::Press(settings::use);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		int i = 0;
		anyLeft = std::any_of(
			gearSlots.begin(), gearSlots.end(), [&i, this](GearSlot slot) {
				return this->HasEquipped(nullptr, static_cast<Slot>(i++));
			});
	}
}

const asa::items::Item* PlayerInfo::GetEquippedItem(Slot slot)
{
	return nullptr;
}
