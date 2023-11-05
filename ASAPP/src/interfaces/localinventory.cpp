#include "localinventory.h"
#include "../_internal/util.h"
#include "../game/controls.h"
#include "../game/settings.h"
#include "exceptions.h"

void asa::interfaces::LocalInventory::Open()
{
	auto start = std::chrono::system_clock::now();
	while (!this->IsOpen()) {
		controls::KeyPress(settings::showMyInventory.key);
		if (internal::_util::Await(
				[this]() { return this->IsOpen(); }, std::chrono::seconds(5))) {
			return;
		}

		if (internal::_util::Timedout(start, std::chrono::seconds(30))) {
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
		if (internal::_util::Await([button]() { return button->IsSelected(); },
				std::chrono::seconds(5))) {
			return;
		}

		if (internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceError(
				this, "Failed to open tab " + std::to_string(tab));
		}
	}
}