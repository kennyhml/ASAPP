#include "asapp/interfaces/playerinfo.h"
#include "../core/wrappers.h"
#include "asapp/game/globals.h"
#include <stdexcept>

namespace asa::interfaces
{
	bool PlayerInfo::is_open() const
	{
		window::Color textColor(188, 244, 255);
		auto mask = window::get_mask(you.area, textColor, 20);
		return cv::countNonZero(mask) > 60;
	}

	const bool PlayerInfo::has_equipped(items::Item* item, Slot slot)
	{
		if (item && (!item->type == item->EQUIPPABLE)) {
			throw std::invalid_argument(
				std::format("Item '{}' cannot be equipped.", item->name));
		}

		GearSlot gear_slot = gear_slots[slot];
		const auto roi = item ? gear_slot
							  : gear_slot.get_slot_description_area();

		if (!item) {
			auto mask = window::get_mask(roi, window::Color(223, 250, 255), 20);
			return cv::countNonZero(mask) < 30;
		}
		else {
			return window::match_template(roi, item->GetInventoryIcon(), 0.7,
				item->GetInventoryIconMask());
		}
	}

	void PlayerInfo::unequip(Slot slot)
	{
		const auto& gear_slot = gear_slots[slot];
		while (has_equipped(nullptr, slot)) {
			auto point = gear_slot.get_random_location(5);
			if (globals::useWindowInput) {
				window::click_at(point, controls::LEFT);
				sleep_for(std::chrono::milliseconds(5));
				window::click_at(point, controls::LEFT);
			}
			else {
				window::set_mouse_pos(point);
				sleep_for(std::chrono::milliseconds(15));
				for (int i = 0; i < 3; i++) {
					controls::press(settings::use);
				}
			}
			sleep_for(std::chrono::milliseconds(10));
		}
	}

	void PlayerInfo::unequip_all()
	{
		bool any_let = true;
		while (any_let) {
			any_let = false;

			for (const auto& slot : gear_slots) {

				auto point = slot.get_random_location(5);
				if (globals::useWindowInput) {
					window::click_at(
						slot.get_random_location(5), controls::LEFT);
					sleep_for(std::chrono::milliseconds(5));
					window::click_at(
						slot.get_random_location(5), controls::LEFT);
				}
				else {
					window::set_mouse_pos(point);
					sleep_for(std::chrono::milliseconds(15));
					for (int i = 0; i < 3; i++) {
						controls::press(settings::use);
					}
				}
				sleep_for(std::chrono::milliseconds(100));
			}
			sleep_for(std::chrono::milliseconds(1000));
			int i = 0;
			any_let = std::any_of(gear_slots.begin(), gear_slots.end(),
				[&i, this](GearSlot slot) {
					return has_equipped(nullptr, static_cast<Slot>(i++));
				});
		}
	}

	const asa::items::Item* PlayerInfo::get_equipped_item(Slot slot)
	{
		return nullptr;
	}

}
