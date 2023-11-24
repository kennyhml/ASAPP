#include "localplayer.h"
#include "../_internal/util.h"
#include "../game/settings.h"
#include "../game/window.h"

using namespace asa::entities;

const bool LocalPlayer::IsInTravelScreen()
{
	static window::Rect roi(806, 436, 310, 219);
	static window::Color white(255, 255, 255);

	auto mask = window::GetMask(roi, white, 5);
	return cv::countNonZero(mask) > 3000;
}

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
}

void LocalPlayer::Access(structures::Container* container)
{
	this->Access(static_cast<structures::InteractableStructure*>(container));

	if (!_internal::_util::Await(
			[container]() {
				return !container->inventory->IsReceivingRemoteInventory();
			},
			std::chrono::seconds(30))) {
		throw std::runtime_error("Failed to receive remote inventory");
	}
}

void LocalPlayer::Access(structures::InteractableStructure* structure)
{
	if (structure->_interface->IsOpen()) {
		return;
	}
	auto start = std::chrono::system_clock::now();
	do {
		window::Press(structure->GetInteractKey(), true);
		if (_internal::_util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access structure");
		}
	} while (!_internal::_util::Await(
		[structure]() { return structure->_interface->IsOpen(); },
		std::chrono::seconds(5)));
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
