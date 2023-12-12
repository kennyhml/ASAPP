#include "asapp/entities/localplayer.h"
#include "../interfaces/hud.h"
#include "../util/util.h"
#include "asapp/core/state.h"
#include "asapp/entities/exceptions.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/structures/exceptions.h"

namespace asa::entities
{
	interfaces::LocalInventory* LocalPlayer::get_inventory() const
	{
		return dynamic_cast<interfaces::LocalInventory*>(inventory.get());
	}

	bool LocalPlayer::is_alive() const
	{
		if (settings::game_user_settings::toggle_hud.get()) {
			window::press(settings::show_extended_info);
		}
		else {
			window::down(settings::show_extended_info);
		}
		bool result = util::await(
			[]() { return interfaces::hud->extended_information_is_toggled(); },
			std::chrono::milliseconds(300));

		if (settings::game_user_settings::toggle_hud.get()) {
			window::press(settings::show_extended_info);
		}
		else {
			window::up(settings::show_extended_info);
		}
		return result;
	}

	bool LocalPlayer::is_out_of_water() const
	{
		return interfaces::hud->is_player_out_of_water();
	}
	bool LocalPlayer::is_out_of_food() const
	{
		return interfaces::hud->is_player_out_of_food();
	}
	bool LocalPlayer::is_overweight() const
	{
		return interfaces::hud->is_player_overweight();
	}

	bool LocalPlayer::received_item(items::Item* item) const
	{
		return interfaces::hud->item_added(item, nullptr);
	}

	bool LocalPlayer::deposited_item(items::Item* item) const
	{
		return interfaces::hud->item_removed(item, nullptr);
	}

	bool LocalPlayer::is_in_travel_screen() const
	{
		static window::Rect roi(94, 69, 1751, 883);
		static window::Color white(255, 255, 255);

		cv::Mat image = window::screenshot(roi);
		cv::Mat gray;
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

		cv::Scalar mean, stddev;
		cv::meanStdDev(gray, mean, stddev);
		return mean[0] > 240.f;
	}

	bool LocalPlayer::can_access_bed() const
	{
		return interfaces::hud->can_fast_travel();
	}

	bool LocalPlayer::can_access_inventory() const
	{
		return interfaces::hud->can_access_inventory();
	}

	bool LocalPlayer::can_use_default_teleport() const
	{
		return interfaces::hud->can_default_teleport();
	}

	bool LocalPlayer::deposit_into_dedi(
		items::Item* item, int* depositedAmountOut)
	{
		auto deposited = [this, item, depositedAmountOut]() {
			if (item && depositedAmountOut) {
				return get_amount_removed(*item, *depositedAmountOut);
			}
			else {
				return deposited_item(item);
			}
		};

		do {
			window::press(settings::use);
		} while (!util::await(deposited, std::chrono::seconds(5)));

		return true;
	}

	bool LocalPlayer::withdraw_from_dedi(
		items::Item* item, int* withdrawnAmountOut)
	{
		return false;
	}

	bool LocalPlayer::get_amount_added(items::Item& item, int& amountOut)
	{
		return interfaces::hud->count_items_added(item, amountOut);
	}

	bool LocalPlayer::get_amount_removed(items::Item& item, int& amountOut)
	{
		return interfaces::hud->count_items_removed(item, amountOut);
	}

	void LocalPlayer::suicide()
	{
		std::cout << "[+] Suiciding with implant..." << std::endl;

		get_inventory()->open();
		controls::mouse_press(controls::LEFT);
		core::sleep_for(std::chrono::milliseconds(100));
		inventory->select_slot(0);

		std::cout << "\t[-] Waiting for implant cooldown... ";
		core::sleep_for(std::chrono::seconds(6));
		std::cout << "Done." << std::endl;

		do {
			window::press(settings::use);
			core::sleep_for(std::chrono::seconds(3));

		} while (is_alive());
		std::cout << "\t[-] Suicided successfully." << std::endl;
	}

	bool LocalPlayer::can_access(const structures::BaseStructure&) const
	{
		return interfaces::hud->can_access_inventory();
		// TODO: if ghud fails use the action wheel
	}

	bool LocalPlayer::can_access(const entities::BaseEntity&) const
	{
		return interfaces::hud->can_access_inventory();
		// TODO: if ghud fails use the action wheel
	}

	void LocalPlayer::access(const entities::BaseEntity& ent) const
	{
		if (ent.get_inventory()->is_open()) {
			return;
		}

		auto start = std::chrono::system_clock::now();
		do {
			window::press(settings::access_inventory, true);
			if (util::timedout(start, std::chrono::seconds(30))) {
				throw EntityNotAccessed(&ent);
			}
		} while (
			!util::await([&ent]() { return ent.get_inventory()->is_open(); },
				std::chrono::seconds(5)));

		ent.get_inventory()->receive_remote_inventory(std::chrono::seconds(30));
	}

	void LocalPlayer::access(const structures::Container& container) const
	{
		// Accessing the inventory is the same as accessing the interface of
		// any interactable structure such as teleporters, beds etc.
		// just that we have to wait to receive the remote inventory afterwards.
		access(static_cast<structures::InteractableStructure>(container));
		container.inventory->receive_remote_inventory(std::chrono::seconds(30));
	}

	void LocalPlayer::access(
		const structures::InteractableStructure& structure) const
	{
		if (structure._interface->is_open()) {
			return;
		}

		auto start = std::chrono::system_clock::now();
		do {
			window::press(structure.get_interact_key(), true);
			if (util::timedout(start, std::chrono::seconds(30))) {
				throw structures::StructureNotOpenedError(&structure);
			}
		} while (!util::await(
			[&structure]() { return structure._interface->is_open(); },
			std::chrono::seconds(5)));
	}

	void LocalPlayer::fast_travel_to(const structures::SimpleBed& bed)
	{
		for (int i = 0; i < 10; i++) {
			turn_down(18, std::chrono::milliseconds(10));
		}
		prone();

		core::sleep_for(std::chrono::milliseconds(300));
		access(bed);
		core::sleep_for(std::chrono::milliseconds(300));

		bed.map->go_to(bed.name);
		pass_travel_screen();
	}

	void LocalPlayer::teleport_to(
		const structures::Teleporter& tp, bool isDefault)
	{
		bool couldAccessBefore = can_access(tp);

		if (!isDefault) {
			look_fully_down();
			access(tp);
			core::sleep_for(std::chrono::milliseconds(500));
			tp.map->go_to(tp.name);
			util::await(
				[]() { return !interfaces::hud->can_default_teleport(); },
				std::chrono::seconds(5));
		}
		else {
			do {
				window::press(settings::reload);
			} while (!util::await(
				[]() { return !interfaces::hud->can_default_teleport(); },
				std::chrono::seconds(5)));
		}
		pass_teleport_screen(!couldAccessBefore);
	}

	void LocalPlayer::lay_on(const structures::SimpleBed& bed)
	{
		while (!bed.action_wheel.is_open()) {
			window::down(settings::use);

			if (!util::await([&bed]() { return bed.action_wheel.is_open(); },
					std::chrono::seconds(3))) {
				window::up(settings::use);
			}
			core::sleep_for(std::chrono::milliseconds(200));
		}
		core::sleep_for(std::chrono::seconds(1));
		bed.action_wheel.select_lay_on();
		window::up(settings::use);
	}

	void LocalPlayer::get_off_bed()
	{
		window::press(settings::use);
		core::sleep_for(std::chrono::seconds(3));
	}

	void LocalPlayer::turn_right(int degrees, std::chrono::milliseconds delay)
	{
		controls::turn_degrees(degrees, 0);
		core::sleep_for(delay);
	}
	void LocalPlayer::turn_left(int degrees, std::chrono::milliseconds delay)
	{
		controls::turn_degrees(-degrees, 0);
		core::sleep_for(delay);
	}

	void LocalPlayer::turn_up(int degrees, std::chrono::milliseconds delay)
	{
		controls::turn_degrees(0, -degrees);
		core::sleep_for(delay);
	}

	void LocalPlayer::turn_down(int degrees, std::chrono::milliseconds delay)
	{
		controls::turn_degrees(0, degrees);
		core::sleep_for(delay);
	}

	void LocalPlayer::equip(
		items::Item* item, interfaces::PlayerInfo::Slot slot)
	{
		bool wasInventoryOpen = inventory->is_open();
		if (!wasInventoryOpen) {
			get_inventory()->open();
			core::sleep_for(std::chrono::milliseconds(500));
		}

		get_inventory()->equip(item, slot);
		if (!wasInventoryOpen) {
			get_inventory()->close();
		}
	}

	void LocalPlayer::unequip(interfaces::PlayerInfo::Slot slot)
	{
		bool wasInventoryOpen = get_inventory()->is_open();
		if (!wasInventoryOpen) {
			get_inventory()->open();
			core::sleep_for(std::chrono::milliseconds(500));
		}
		get_inventory()->info.unequip(slot);
		if (!wasInventoryOpen) {
			get_inventory()->close();
		}
	}

	void LocalPlayer::pass_travel_screen(bool in, bool out)
	{
		if (in) {
			if (!util::await([this]() { return is_in_travel_screen(); },
					std::chrono::seconds(30))) {
			}
		}

		if (out) {
			if (!util::await([this]() { return !is_in_travel_screen(); },
					std::chrono::seconds(30))) {
			}
		}

		core::sleep_for(std::chrono::seconds(1));
	}

	void LocalPlayer::pass_teleport_screen(bool allowAccessFlag)
	{
		while (!interfaces::hud->can_default_teleport()) {
			// for long distance teleports we still enter a white screen,
			// so we can simply reuse our bed logic
			if (is_in_travel_screen()) {
				std::cout << "[+] Whitescreen entered upon teleport."
						  << std::endl;
				return pass_travel_screen(false);
			}
			if (allowAccessFlag && can_access_inventory()) {
				std::cout << "[+] Teleported to a container." << std::endl;
				return;
			}
		}
		// See whether the default teleport popup lasts for more than 1 second
		// if it doesnt its a glitched popup that appears when the teleport has
		// happened. Restart the procedure in that case
		if (util::await(
				[]() { return !interfaces::hud->can_default_teleport(); },
				std::chrono::milliseconds(1000))) {
			std::cout << "[!] Glitched default teleport popup found."
					  << std::endl;
			return pass_teleport_screen();
		}
	}

	void LocalPlayer::look_fully_down()
	{
		for (int i = 0; i < 10; i++) {
			turn_down(18, std::chrono::milliseconds(10));
		}
		core::sleep_for(std::chrono::milliseconds(300));
	}

	void LocalPlayer::look_fully_up()
	{
		for (int i = 0; i < 10; i++) {
			turn_up(18, std::chrono::milliseconds(10));
		}
		core::sleep_for(std::chrono::milliseconds(300));
	}

}
