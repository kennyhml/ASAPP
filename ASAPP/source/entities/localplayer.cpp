#include "asapp/entities/localplayer.h"
#include "../interfaces/hud.h"
#include "../util/util.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/structures/exceptions.h"

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
	static window::Rect roi(94, 69, 1751, 883);
	static window::Color white(255, 255, 255);

	cv::Mat image = window::Screenshot(roi);
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	cv::Scalar mean, stddev;
	cv::meanStdDev(gray, mean, stddev);
	return mean[0] > 240.f;
}

const bool LocalPlayer::LocalPlayer::CanAccessBed()
{
	return interfaces::gHUD->CanFastTravel();
}

const bool LocalPlayer::LocalPlayer::CanUseDefaultTeleport()
{
	return interfaces::gHUD->CanDefaultTeleport();
}

const bool LocalPlayer::DepositIntoDedicatedStorage(int* depositedAmountOut)
{
	do {
		window::Press(settings::use);
	} while (!util::Await(
		[this]() { return this->DepositedItem(); }, std::chrono::seconds(5)));

	return true;
}

const bool LocalPlayer::WithdrawFromDedicatedStorage(int* withdrawnAmountOut)
{
	return false;
}

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

const bool LocalPlayer::CanAccess(const structures::BaseStructure&)
{
	return interfaces::gHUD->CanAccessInventory();
	// TODO: if ghud fails use the action wheel
}

const bool LocalPlayer::CanAccess(const entities::BaseEntity&)
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
			throw std::runtime_error("Failed to access dino");
		}
	} while (!util::Await(
		[ent]() { return ent.inventory->IsOpen(); }, std::chrono::seconds(5)));

	if (!util::Await(
			[ent]() { return !ent.inventory->IsReceivingRemoteInventory(); },
			std::chrono::seconds(30))) {
		throw std::runtime_error("Failed to receive remote inventory");
	}
}

void LocalPlayer::Access(const structures::Container& container)
{
	this->Access(static_cast<structures::InteractableStructure>(container));

	if (!util::Await(
			[container]() {
				return !container.inventory->IsReceivingRemoteInventory();
			},
			std::chrono::seconds(30))) {
		throw structures::exceptions::StructureError(
			&container, "Failed to receive remote inventory");
	}
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

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	this->Access(bed);
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	bed.map->GoTo(bed.name);
	this->PassTravelScreen();
}

void LocalPlayer::TeleportTo(const structures::Teleporter& tp, bool isDefault)
{
	if (!isDefault) {
		this->LookAllTheWayDown();
		this->Access(tp);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
	this->PassTeleportScreen();
}

void LocalPlayer::LayOn(const structures::SimpleBed& bed)
{
	while (!bed.actionWheel.IsOpen()) {
		window::Down(settings::use);

		if (!util::Await([bed]() { return bed.actionWheel.IsOpen(); },
				std::chrono::seconds(3))) {
			window::Up(settings::use);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	bed.actionWheel.SelectLayOn();
	window::Up(settings::use);
}

void LocalPlayer::GetOffBed()
{
	window::Press(settings::use);
	std::this_thread::sleep_for(std::chrono::seconds(3));
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

	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void LocalPlayer::PassTeleportScreen()
{
	while (!interfaces::gHUD->CanDefaultTeleport()) {
		// for long distance teleports we still enter a white screen,
		// so we can simply reuse our bed logic
		if (this->IsInTravelScreen()) {
			std::cout << "[+] Whitescreen entered upon teleport." << std::endl;
			return this->PassTravelScreen(false);
		}
	}
	// See whether the default teleport popup lasts for more than 1 second
	// if it doesnt its a glitched popup that appears when the teleport has
	// happened. Restart the procedure in that case
	if (util::Await([]() { return !interfaces::gHUD->CanDefaultTeleport(); },
			std::chrono::milliseconds(800))) {
		std::cout << "[!] Glitched default teleport popup found." << std::endl;
		return this->PassTeleportScreen();
	}
}

void LocalPlayer::LookAllTheWayDown()
{
	for (int i = 0; i < 10; i++) {
		this->TurnDown(18, std::chrono::milliseconds(10));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void LocalPlayer::LookAllTheWayUp()
{
	for (int i = 0; i < 10; i++) {
		this->TurnUp(18, std::chrono::milliseconds(10));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}