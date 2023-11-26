#include "asapp/entities/localplayer.h"
#include "../interfaces/hud.h"
#include "../util/util.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"

using namespace asa::entities;

const bool LocalPlayer::IsAlive()
{
	if (settings::gameUserSettings::toggleHUD.get()) {
		window::Press(settings::showExtendedInfo);
	}
	else {
		window::Down(settings::showExtendedInfo);
	}
	bool result = util::Await(
		[]() { return interfaces::gHUD->ExtendedInformationIsToggled(); },
		std::chrono::milliseconds(300));

	if (settings::gameUserSettings::toggleHUD.get()) {
		window::Press(settings::showExtendedInfo);
	}
	else {
		window::Up(settings::showExtendedInfo);
	}
	return result;
}

const bool LocalPlayer::IsOutOfWater()
{
	return interfaces::gHUD->IsPlayerOutOfWater();
}
const bool LocalPlayer::IsOutOfFood()
{
	return interfaces::gHUD->IsPlayerOutOfFood();
}
const bool LocalPlayer::IsOverweight()
{
	return interfaces::gHUD->IsPlayerOverweight();
}

const bool LocalPlayer::ReceivedItem()
{
	return interfaces::gHUD->GotItemAdded(this->inventory->IsOpen());
}

const bool LocalPlayer::DepositedItem()
{
	return interfaces::gHUD->GotItemRemoved(this->inventory->IsOpen());
}

const bool LocalPlayer::LocalPlayer::IsInTravelScreen()
{
	static window::Rect roi(806, 436, 310, 219);
	static window::Color white(255, 255, 255);

	auto mask = window::GetMask(roi, white, 5);
	return cv::countNonZero(mask) > 3000;
}

const bool LocalPlayer::DepositIntoDedicatedStorage(int* depositedAmountOut)
{
	do {
		window::Press(settings::use);
	} while (!util::Await(
		[this]() { return this->DepositedItem(); }, std::chrono::seconds(5)));

	return true;
}

const bool LocalPlayer::WithdrawFromDedicatedStorage(int* withdrawnAmountOut) {}

void LocalPlayer::Suicide()
{
	std::cout << "[+] Suiciding with implant..." << std::endl;

	this->inventory->Open();
	controls::MousePress(controls::LEFT);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	this->inventory->SelectSlot(0);

	std::cout << "\t[-] Waiting for implant cooldown... ";
	std::this_thread::sleep_for(std::chrono::seconds(6));
	std::cout << "Done." << std::endl;

	do {
		window::Press(settings::use);
		std::this_thread::sleep_for(std::chrono::seconds(3));

	} while (this->IsAlive());
	std::cout << "\t[-] Suicided successfully." << std::endl;
}

void LocalPlayer::Access(entities::BaseEntity* ent)
{
	if (ent->inventory->IsOpen()) {
		return;
	}

	auto start = std::chrono::system_clock::now();
	do {
		window::Press(settings::accessInventory, true);
		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access dino");
		}
	} while (!util::Await(
		[ent]() { return ent->inventory->IsOpen(); }, std::chrono::seconds(5)));

	if (!util::Await(
			[ent]() { return !ent->inventory->IsReceivingRemoteInventory(); },
			std::chrono::seconds(30))) {
		throw std::runtime_error("Failed to receive remote inventory");
	}
}

void LocalPlayer::Access(structures::Container* container)
{
	this->Access(static_cast<structures::InteractableStructure*>(container));

	if (!util::Await(
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
		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw std::runtime_error("Failed to access structure");
		}
	} while (
		!util::Await([structure]() { return structure->_interface->IsOpen(); },
			std::chrono::seconds(5)));
}

void LocalPlayer::FastTravelTo(structures::SimpleBed* bed)
{
	for (int i = 0; i < 10; i++) {
		this->TurnDown(18, std::chrono::milliseconds(10));
	}
	this->Prone();

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	this->Access(bed);
	bed->map->GoTo(bed->name);
	this->PassTravelScreen();
}

void LocalPlayer::TurnRight(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(degrees, 0);
	std::this_thread::sleep_for(delay);
}
void LocalPlayer::TurnLeft(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(-degrees, 0);
	std::this_thread::sleep_for(delay);
}

void LocalPlayer::TurnUp(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(0, -degrees);
	std::this_thread::sleep_for(delay);
}

void LocalPlayer::TurnDown(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(0, degrees);
	std::this_thread::sleep_for(delay);
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

void LocalPlayer::PassTravelScreen()
{
	if (!util::Await([this]() { return this->IsInTravelScreen(); },
			std::chrono::seconds(30))) {
		// throw a timeout error or something
	}

	if (!util::Await([this]() { return !this->IsInTravelScreen(); },
			std::chrono::seconds(30))) {
		// throw a timeout error or something
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
}