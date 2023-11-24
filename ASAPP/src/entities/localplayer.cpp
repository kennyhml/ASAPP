#include "localplayer.h"
#include "../_internal/util.h"
#include "../game/settings.h"
#include "../game/window.h"

using namespace asa::entities;

const bool LocalPlayer::DepositIntoDedicatedStorage(int* depositedAmountOut) {}

const bool LocalPlayer::WithdrawFromDedicatedStorage(int withdrawnAmountOut) {}

void LocalPlayer::Access(entities::BaseEntity* ent)
{
	if (ent->inventory->IsOpen()) {
		return;
	}

	auto start = std::chrono::system_clock::now();
	do {
		window::Press(settings::accessInventory, true);
		if (_internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access dino");
		}
	} while (!_internal::_util::Await(
		[ent]() { return ent->inventory->IsOpen(); }, std::chrono::seconds(5)));

	if (!_internal::_util::Await(
			[ent]() { return !ent->inventory->IsReceivingRemoteInventory(); },
			std::chrono::seconds(30))) {
		throw std::runtime_error("Failed to receive remote inventory");
	}
	std::cout << "Inventory open" << std::endl;
}

void LocalPlayer::Access(structures::BaseStructure* structure)
{
	if (structure->inventory->IsOpen()) {
		return;
	}
	auto start = std::chrono::system_clock::now();
	do {
		window::Press(settings::accessInventory, true);
		if (_internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access structure");
		}
	} while (!_internal::_util::Await(
		[structure]() { return structure->inventory->IsOpen(); },
		std::chrono::seconds(5)));

	if (!_internal::_util::Await(
			[structure]() {
				return structure->inventory->IsReceivingRemoteInventory();
			},
			std::chrono::seconds(30))) {
		throw std::runtime_error("Failed to receive remote inventory");
	}
}


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
