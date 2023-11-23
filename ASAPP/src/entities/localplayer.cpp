#include "localplayer.h"
#include "../game/settings.h"
#include "../game/window.h"

using namespace asa::entities;

void LocalPlayer::Jump() { window::Press(settings::actionMappings::jump); }

void LocalPlayer::Equip(
	items::Item* item, interfaces::PlayerInfo::Slot targetSlot)
{
	this->inventory->Open();
	this->inventory->Equip(item, targetSlot);
	this->inventory->Close();
}