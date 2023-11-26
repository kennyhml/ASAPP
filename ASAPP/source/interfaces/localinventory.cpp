#include "asapp/interfaces/localinventory.h"
#include "../util/util.h"
#include "asapp/game/controls.h"
#include "asapp/game/settings.h"
#include "asapp/interfaces/exceptions.h"

void asa::interfaces::LocalInventory::Open()
{
	auto start = std::chrono::system_clock::now();
	while (!this->IsOpen()) {
		window::Press(settings::showMyInventory, true);
		if (util::Await(
				[this]() { return this->IsOpen(); }, std::chrono::seconds(5))) {
			break;
		}

		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceNotOpenedError(this);
		}
	}
}

void asa::interfaces::LocalInventory::SwitchTo(Tab tab)
{
	InvTabButton* button = nullptr;
	switch (tab) {
	case Tab::INVENTORY:
		button = &inventoryTab;
		break;
	case Tab::COSMETICS:
		button = &cosmeticsTab;
		break;
	case Tab::CRAFTING:
		button = &craftingTab;
		break;
	}
	assert(button != nullptr);

	auto start = std::chrono::system_clock::now();
	while (!button->IsSelected()) {
		button->Press();
		if (util::Await([button]() { return button->IsSelected(); },
				std::chrono::seconds(5))) {
			return;
		}

		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceError(
				this, "Failed to open tab " + std::to_string(tab));
		}
	}
}
void asa::interfaces::LocalInventory::Equip(
	items::Item* item, PlayerInfo::Slot slot)
{
	bool searched = false;
	if (!this->Has(item, false)) {
		searched = true;
		if (!this->Has(item, true)) {
			throw std::runtime_error(
				std::format("No '{}' in local player inventory", item->name));
		}
	}

	const Slot* itemLocation = this->FindItem(item, searched);
	this->SelectSlot(*itemLocation);

	do {
		window::Press(settings::actionMappings::use);
	} while (!util::Await(
		[this, item, slot]() { return this->info.HasEquipped(item, slot); },
		std::chrono::seconds(5)));
}
