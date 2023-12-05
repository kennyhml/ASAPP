#include "asapp/entities/localplayer.h"
#include "../core/wrappers.h"
#include "../interfaces/hud.h"
#include "../util/util.h"
#include "asapp/entities/exceptions.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/structures/exceptions.h"

using namespace asa::entities;

bool LocalPlayer::IsAlive()
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

bool LocalPlayer::IsOutOfWater()
{
	return interfaces::gHUD->IsPlayerOutOfWater();
}
bool LocalPlayer::IsOutOfFood()
{
	return interfaces::gHUD->IsPlayerOutOfFood();
}
bool LocalPlayer::IsOverweight()
{
	return interfaces::gHUD->IsPlayerOverweight();
}

bool LocalPlayer::ReceivedItem()
{
	return interfaces::gHUD->GotItemAdded(this->inventory->IsOpen());
}

bool LocalPlayer::DepositedItem()
{
	return interfaces::gHUD->GotItemRemoved(this->inventory->IsOpen());
}

bool LocalPlayer::LocalPlayer::IsInTravelScreen()
{
	static window::Rect roi(94, 69, 1751, 883);
	static window::Color white(255, 255, 255);

	cv::Mat image = window::Screenshot(roi);
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	cv::Scalar mean, stddev;
	cv::meanStdDev(gray, mean, stddev);
	return mean[0] > 240.f;
}

bool LocalPlayer::LocalPlayer::CanAccessBed()
{
	return interfaces::gHUD->CanFastTravel();
}

bool LocalPlayer::LocalPlayer::CanAccessInventory()
{
	return interfaces::gHUD->CanAccessInventory();
}

bool LocalPlayer::LocalPlayer::CanUseDefaultTeleport()
{
	return interfaces::gHUD->CanDefaultTeleport();
}

bool LocalPlayer::DepositIntoDedicatedStorage(int* depositedAmountOut)
{
	do {
		window::Press(settings::use);
	} while (!util::Await(
		[this]() { return this->DepositedItem(); }, std::chrono::seconds(5)));

	return true;
}

bool LocalPlayer::WithdrawFromDedicatedStorage(int* withdrawnAmountOut)
{
	return false;
}

void LocalPlayer::Suicide()
{
	std::cout << "[+] Suiciding with implant..." << std::endl;

	this->inventory->Open();
	controls::MousePress(controls::LEFT);
	SleepFor(std::chrono::milliseconds(100));
	this->inventory->SelectSlot(0);

	std::cout << "\t[-] Waiting for implant cooldown... ";
	SleepFor(std::chrono::seconds(6));
	std::cout << "Done." << std::endl;

	do {
		window::Press(settings::use);
		SleepFor(std::chrono::seconds(3));

	} while (this->IsAlive());
	std::cout << "\t[-] Suicided successfully." << std::endl;
}

bool LocalPlayer::CanAccess(const structures::BaseStructure&)
{
	return interfaces::gHUD->CanAccessInventory();
	// TODO: if ghud fails use the action wheel
}

bool LocalPlayer::CanAccess(const entities::BaseEntity&)
{
	return interfaces::gHUD->CanAccessInventory();
	// TODO: if ghud fails use the action wheel
}

void LocalPlayer::Access(const entities::BaseEntity& ent)
{
	if (ent.inventory->IsOpen()) {
		return;
	}

	auto start = std::chrono::system_clock::now();
	do {
		window::Press(settings::accessInventory, true);
		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::EntityNotAccessed(&ent);
		}
	} while (!util::Await(
		[ent]() { return ent.inventory->IsOpen(); }, std::chrono::seconds(5)));

	ent.inventory->ReceiveRemoteInventory(std::chrono::seconds(30));
}

void LocalPlayer::Access(const structures::Container& container)
{
	// Accessing the inventory is the same as accessing the interface of
	// any interactable structure such as teleporters, beds etc.
	// just that we have to wait to receive the remote inventory afterwards.
	this->Access(static_cast<structures::InteractableStructure>(container));
	container.inventory->ReceiveRemoteInventory(std::chrono::seconds(30));
}

void LocalPlayer::Access(const structures::InteractableStructure& structure)
{
	if (structure._interface->IsOpen()) {
		return;
	}

	auto start = std::chrono::system_clock::now();
	do {
		window::Press(structure.GetInteractKey(), true);
		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw structures::exceptions::StructureError(&structure);
		}
	} while (
		!util::Await([structure]() { return structure._interface->IsOpen(); },
			std::chrono::seconds(5)));
}

void LocalPlayer::FastTravelTo(const structures::SimpleBed& bed)
{
	for (int i = 0; i < 10; i++) {
		this->TurnDown(18, std::chrono::milliseconds(10));
	}
	this->Prone();

	SleepFor(std::chrono::milliseconds(300));
	this->Access(bed);
	SleepFor(std::chrono::milliseconds(300));

	bed.map->GoTo(bed.name);
	this->PassTravelScreen();
}

void LocalPlayer::TeleportTo(const structures::Teleporter& tp, bool isDefault)
{
	bool couldAccessBefore = this->CanAccess(tp);

	if (!isDefault) {
		this->LookAllTheWayDown();
		this->Access(tp);
		SleepFor(std::chrono::milliseconds(500));
		tp.map->GoTo(tp.name);
		util::Await([]() { return !interfaces::gHUD->CanDefaultTeleport(); },
			std::chrono::seconds(5));
	}
	else {
		do {
			window::Press(settings::reload);
		} while (!util::Await(
			[]() { return !interfaces::gHUD->CanDefaultTeleport(); },
			std::chrono::seconds(5)));
	}
	this->PassTeleportScreen(!couldAccessBefore);
}

void LocalPlayer::LayOn(const structures::SimpleBed& bed)
{
	while (!bed.actionWheel.IsOpen()) {
		window::Down(settings::use);

		if (!util::Await([bed]() { return bed.actionWheel.IsOpen(); },
				std::chrono::seconds(3))) {
			window::Up(settings::use);
		}
		SleepFor(std::chrono::milliseconds(200));
	}
	SleepFor(std::chrono::seconds(1));
	bed.actionWheel.SelectLayOn();
	window::Up(settings::use);
}

void LocalPlayer::GetOffBed()
{
	window::Press(settings::use);
	SleepFor(std::chrono::seconds(3));
}

void LocalPlayer::TurnRight(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(degrees, 0);
	SleepFor(delay);
}
void LocalPlayer::TurnLeft(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(-degrees, 0);
	SleepFor(delay);
}

void LocalPlayer::TurnUp(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(0, -degrees);
	SleepFor(delay);
}

void LocalPlayer::TurnDown(int degrees, std::chrono::milliseconds delay)
{
	controls::TurnDegrees(0, degrees);
	SleepFor(delay);
}

void LocalPlayer::Equip(
	items::Item* item, interfaces::PlayerInfo::Slot targetSlot)
{
	bool wasInventoryOpen = this->inventory->IsOpen();
	if (!wasInventoryOpen) {
		this->inventory->Open();
		SleepFor(std::chrono::milliseconds(500));
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
		SleepFor(std::chrono::milliseconds(500));
	}
	this->inventory->info.Unequip(targetSlot);
	if (!wasInventoryOpen) {
		this->inventory->Close();
	}
}

void LocalPlayer::PassTravelScreen(bool in, bool out)
{
	if (in) {
		if (!util::Await([this]() { return this->IsInTravelScreen(); },
				std::chrono::seconds(30))) {
		}
	}

	if (out) {
		if (!util::Await([this]() { return !this->IsInTravelScreen(); },
				std::chrono::seconds(30))) {
		}
	}

	SleepFor(std::chrono::seconds(1));
}

void LocalPlayer::PassTeleportScreen(bool allowAccessFlag)
{
	while (!interfaces::gHUD->CanDefaultTeleport()) {
		// for long distance teleports we still enter a white screen,
		// so we can simply reuse our bed logic
		if (this->IsInTravelScreen()) {
			std::cout << "[+] Whitescreen entered upon teleport." << std::endl;
			return this->PassTravelScreen(false);
		}
		if (allowAccessFlag && this->CanAccessInventory()) {
			std::cout << "[+] Teleported to a container." << std::endl;
			return;
		}
	}
	// See whether the default teleport popup lasts for more than 1 second
	// if it doesnt its a glitched popup that appears when the teleport has
	// happened. Restart the procedure in that case
	if (util::Await([]() { return !interfaces::gHUD->CanDefaultTeleport(); },
			std::chrono::milliseconds(1000))) {
		std::cout << "[!] Glitched default teleport popup found." << std::endl;
		return this->PassTeleportScreen();
	}
}

void LocalPlayer::LookAllTheWayDown()
{
	for (int i = 0; i < 10; i++) {
		this->TurnDown(18, std::chrono::milliseconds(10));
	}
	SleepFor(std::chrono::milliseconds(300));
}

void LocalPlayer::LookAllTheWayUp()
{
	for (int i = 0; i < 10; i++) {
		this->TurnUp(18, std::chrono::milliseconds(10));
	}
	SleepFor(std::chrono::milliseconds(300));
}