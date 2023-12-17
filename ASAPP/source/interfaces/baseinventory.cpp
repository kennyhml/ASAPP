#include "asapp/interfaces/baseinventory.h"
#include "../util/util.h"
#include "asapp/core/state.h"
#include "asapp/game/controls.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
	BaseInventory::BaseInventory(bool r)
		: area(r ? 1179 : 149, 94, 591, 827), is_remote_inventory(r),
		  item_filter(r ? 1205 : 175, 841, 552, 42),
		  search_bar(r ? 1207 : 177, 176, 142, 44),
		  transfer_all_button(r ? 1388 : 366, 176),
		  drop_all_button(r ? 1436 : 413, 176),
		  new_folder_button(r ? 1531 : 509, 176),
		  auto_stack_button(r ? 1579 : 557, 176),
		  folder_view_button(r ? 1652 : 629, 176),
		  item_area(r ? 1205 : 178, 239, 552, 588)
	{
		init_slots({ r ? 1205 : 178, 239 });
	};

	[[nodiscard]] bool BaseInventory::ManagementButton::is_toggled() const
	{
		window::Color toggled_color(128, 231, 255);
		auto masked = window::get_mask(this->area, toggled_color, 10);
		return cv::countNonZero(masked) > 30;
	}

	[[nodiscard]] bool BaseInventory::ManagementButton::is_available() const
	{
		window::Color base_color(0, 140, 171);
		auto masked = window::get_mask(this->area, base_color, 10);
		return cv::countNonZero(masked) > 20;
	}

	[[nodiscard]] bool BaseInventory::InvTabButton::is_selected() const
	{
		window::Color selected_color(188, 244, 255);
		auto masked = window::get_mask(this->area, selected_color, 10);
		return cv::countNonZero(masked) > 100;
	}

	[[nodiscard]] bool BaseInventory::InvTabButton::exists() const
	{
		window::Color inactive_color(80, 141, 155);
		auto masked = window::get_mask(this->area, inactive_color, 10);
		return cv::countNonZero(masked) > 100 || this->is_selected();
	}

	[[nodiscard]] bool BaseInventory::Slot::has_item() const
	{
		auto roi = window::Rect(this->x + 46, this->y + 69, 42, 14);
		window::Color weight_text_col(128, 231, 255);

		cv::Mat masked = window::get_mask(roi, weight_text_col, 35);
		return cv::countNonZero(masked) > 10;
	}

	[[nodiscard]] bool BaseInventory::Slot::has_item(items::Item* item) const
	{
		if (!item) {
			return this->has_item();
		}
		return window::match_template(*this, item->get_inventory_icon(), 0.7,
			item->get_inventory_icon_mask());
	}

	bool BaseInventory::is_receiving_remote_inventory() const
	{
		if (!this->is_remote_inventory) {
			return false;
		}

		window::Color text_color(191, 243, 255);
		auto mask = window::get_mask(
			this->recv_remote_inventory_area, text_color, 25);
		return cv::countNonZero(mask) > 100;
	}

	void BaseInventory::receive_remote_inventory(
		std::chrono::seconds timeout) const
	{
		auto start = std::chrono::system_clock::now();

		if (!util::await(
				[this]() { return !this->is_receiving_remote_inventory(); },
				timeout)) {
			throw ReceivingRemoteInventoryTimeoutError(this);
		}
	}

	bool BaseInventory::is_open() const
	{
		return window::match_template(
			this->item_filter.area, resources::interfaces::cb_arrowdown, 0.9);
	}

	bool BaseInventory::has(items::Item* item, bool search)
	{
		if (search) {
			this->search_bar.search_for(item->name);
			Sleep(100);
		}

		// if an items query isnt ambigious, i.e when we enter the item name
		// ONLY the item can show up, just check the first slot for
		// efficiency.
		if (search && !item->has_ambiguous_query) {
			return this->slots[0].has_item(item);
		}

		return window::match_template(this->item_area,
			item->get_inventory_icon(), 0.7, item->get_inventory_icon_mask());
	}

	bool BaseInventory::count_stacks(
		items::Item* item, int& stacks_out, bool search)
	{
		if (search && !this->has(item)) {
			this->search_bar.search_for(item->name);
			Sleep(100);
		}

		auto matches = window::locate_all_template(this->item_area,
			item->get_inventory_icon(), 0.9, item->get_inventory_icon_mask());

		if (matches.empty()) {
			stacks_out = 0;
			return true;
		}
		stacks_out = matches.size();
		return stacks_out != MAX_ITEMS_PER_PAGE;
	}

	const BaseInventory::Slot* BaseInventory::find_item(
		items::Item* item, bool is_searched, bool search_for)
	{
		if (!item->has_ambiguous_query && (is_searched || search_for)) {
			if (search_for) {
				this->search_bar.search_for(item->name);
			}
			return this->slots[0].has_item(item) ? &this->slots[0] : nullptr;
		}

		if (search_for) {
			this->search_bar.search_for(item->name);
		}
		for (const Slot& slot : this->slots) {
			if (slot.has_item(item)) {
				return &slot;
			}
			else if (!slot.has_item()) {
				return nullptr;
			}
		}
		return nullptr;
	}
	asa::window::Rect BaseInventory::get_area() const { return this->area; }

	void BaseInventory::init_slots(const window::Point& origin)
	{
		int x = origin.x;
		int y = origin.y;

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				this->slots[(i * 6) + j] = Slot(x + (j * 93), y + (i * 93));
			}
		}
	}

	void BaseInventory::popcorn(items::Item* item)
	{
		int tmp{ 0 };
		this->popcorn(item, -1, tmp);
	}

	void BaseInventory::popcorn(items::Item* item, int stacks)
	{
		int tmp{ 0 };
		this->popcorn(item, stacks, tmp);
	}

	void BaseInventory::popcorn(
		items::Item* item, int stacks, int& stacks_dropped)
	{
		int dropped = 0;

		if (!this->search_bar.is_text_entered()) {
			this->search_bar.search_for(item->name);
		}

		while (this->slots[0].has_item(item) &&
			   (dropped < stacks || stacks == -1)) {
			for (int i = 0; i < 4; i++) {
				window::set_mouse_pos(this->slots[i].get_random_location(5));
				Sleep(20);
				controls::key_press(settings::action_mappings::drop_item.key);
				Sleep(100);
				dropped++;
			}
		}

		stacks_dropped = dropped;
	}

	void BaseInventory::popcorn_slots(int slots)
	{
		for (int slot = slots - 1; slot >= 0; slot--) {
			window::set_mouse_pos(this->slots[slot].get_random_location(5));
			Sleep(20);
			controls::key_press(settings::action_mappings::drop_item.key);
			Sleep(100);
		}
	}

	void BaseInventory::take_slot(Slot slot)
	{
		this->select_slot(slot);
		window::post_mouse_press_at(
			slot.get_random_location(5), controls::LEFT);
		core::sleep_for(std::chrono::milliseconds(10));
		window::press(
			settings::transfer_item, false, std::chrono::milliseconds(15));
		core::sleep_for(std::chrono::milliseconds(50));
	}

	void BaseInventory::take_slot(int index) { take_slot(this->slots[index]); }

	void BaseInventory::close()
	{
		auto start = std::chrono::system_clock::now();
		while (this->is_open()) {
			window::press("esc", true);
			if (util::await([this]() { return !this->is_open(); },
					std::chrono::seconds(5))) {
				return;
			}

			if (util::timedout(start, std::chrono::seconds(30))) {
				throw InterfaceNotClosedError(this);
			}
		}
	}

	void BaseInventory::select_slot(Slot slot)
	{
		window::Point location = slot.get_random_location(5);
		window::set_mouse_pos(location);
		core::sleep_for(std::chrono::milliseconds(50));
	}

	void BaseInventory::select_slot(int index)
	{
		return select_slot(this->slots[index]);
	}

	void BaseInventory::drop_all()
	{
		this->drop_all_button.press();
		this->search_bar.set_text_cleared();
		Sleep(200);
		// TO DO: Wait for the items to be dropped
	}

	void BaseInventory::transfer_all(items::Item* item, BaseInventory* tar)
	{
		if (item) {
			this->search_bar.search_for(item->name);
			core::sleep_for(std::chrono::milliseconds(50));
		}

		this->transfer_all_button.press();
		this->search_bar.set_text_cleared();
		Sleep(200);
		// TO DO: Wait for the items to be transferred
	}

	void BaseInventory::transfer_all(
		const std::string& term, BaseInventory* tar)
	{
		this->search_bar.search_for(term);
		core::sleep_for(std::chrono::milliseconds(50));

		return this->transfer_all(nullptr, tar);
	}

	void BaseInventory::transfer(
		items::Item* item, int amount, BaseInventory*, bool search)
	{
		if (search) {
			this->search_bar.search_for(item->name);
			core::sleep_for(std::chrono::milliseconds(50));
		}

		int i = 0;
		while (auto slot = this->find_item(item, search)) {
			if (i++ > amount && amount != 0) {
				break;
			}
			this->take_slot(*slot);
		}
	}

	void BaseInventory::auto_stack()
	{
		auto_stack_button.press();
	}
}