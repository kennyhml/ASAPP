#include "localinventory.h"
#include "../game/controls.h"
#include "../game/settings.h"

void asa::interfaces::LocalInventory::Open()
{
	auto start = std::chrono::system_clock::now();

	while (!this->IsOpen()) {
		controls::KeyPress(settings::showMyInventory.key);
		if (this->AwaitOpen(5)) {
			return;
		}
		auto now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<seconds>(now - start).count() > 30) {
			throw std::runtime_error("Failed to open interface!");
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

	int i = 0;
	while (!button->IsSelected()) {
		if (i % 25 == 0) {
			button->Press();
		}
		Sleep(20);
	}
}