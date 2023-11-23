#include "localplayer.h"
#include "../game/settings.h"
#include "../game/window.h"

using namespace asa::entities;


void LocalPlayer::Equip(
	items::Item* item, interfaces::PlayerInfo::Slot targetSlot)
{
	bool wasInventoryOpen = this->inventory->IsOpen();
	if (!wasInventoryOpen) {
		this->inventory->Open();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}


	this->inventory->Equip(item, targetSlot);
	if (!wasInventoryOpen) {
		this->inventory->Close();
	}
}

void LocalPlayer::Unequip(interfaces::PlayerInfo::Slot targetSlot)
{
	bool wasInventoryOpen = this->inventory->IsOpen();
	if (!wasInventoryOpen) {
		this->inventory->Open();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	this->inventory->info.Unequip(targetSlot);
	if (!wasInventoryOpen) {
		this->inventory->Close();
	}
}
