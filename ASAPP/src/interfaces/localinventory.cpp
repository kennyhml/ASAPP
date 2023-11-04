#include "localinventory.h"
#include "../game/controls.h"
#include "../game/settings.h"

bool asa::interfaces::LocalInventory::Open()
{
	int i = 0;
	while (!this->IsOpen()) {
		if (i % 500 == 0) {
			controls::KeyPress(settings::showMyInventory.key);
		}
		Sleep(20);
		i++;
	}
	return true;
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