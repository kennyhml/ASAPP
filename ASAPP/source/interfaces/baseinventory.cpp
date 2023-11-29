#include "asapp/interfaces/baseinventory.h"
#include "../util/util.h"
#include "asapp/game/controls.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"

using namespace asa::interfaces;

[[nodiscard]] bool BaseInventory::ManagementButton::IsToggled() const
{
	window::Color toggledColor(128, 231, 255);
	auto masked = window::GetMask(this->area, toggledColor, 10);
	return cv::countNonZero(masked) > 30;
}

[[nodiscard]] bool BaseInventory::ManagementButton::IsAvailable() const
{
	window::Color baseColor(0, 140, 171);
	auto masked = window::GetMask(this->area, baseColor, 10);
	return cv::countNonZero(masked) > 20;
}

[[nodiscard]] bool BaseInventory::InvTabButton::IsSelected() const
{
	window::Color selectedColor(188, 244, 255);
	auto masked = window::GetMask(this->area, selectedColor, 10);
	return cv::countNonZero(masked) > 100;
}

[[nodiscard]] bool BaseInventory::InvTabButton::Exists() const
{
	window::Color inactiveColor(80, 141, 155);
	auto masked = window::GetMask(this->area, inactiveColor, 10);
	return cv::countNonZero(masked) > 100 || this->IsSelected();
}

[[nodiscard]] bool BaseInventory::Slot::HasItem() const
{
	auto roi = window::Rect(this->x + 46, this->y + 69, 42, 14);
	window::Color weightTextCol(128, 231, 255);

	cv::Mat masked = window::GetMask(roi, weightTextCol, 35);
	return cv::countNonZero(masked) > 10;
}

[[nodiscard]] bool BaseInventory::Slot::HasItem(items::Item* item) const
{
	if (!item) {
		return this->HasItem();
	}
	return window::MatchTemplate(*this, item->icon);
}

bool BaseInventory::IsReceivingRemoteInventory()
{
	if (!this->isRemoteInventory) {
		return false;
	}

	window::Color textColor(191, 243, 255);
	auto mask = window::GetMask(this->recvRemoteInventoryArea, textColor, 25);
	return cv::countNonZero(mask) > 100;
}

bool BaseInventory::IsOpen()
{
	return window::MatchTemplate(
		this->itemFilter.area, resources::cb_arrowdown, 0.9);
}

bool BaseInventory::Has(items::Item* item, bool search)
{
	if (search) {
		this->searchBar.SearchFor(item->name);
		Sleep(100);
	}

	// if an items query isnt ambigious, i.e when we enter the item name
	// ONLY the item can show up, just check the first slot for efficiency.
	if (search && !item->hasAmbigiousQuery) {
		return this->slots[0].HasItem(item);
	}

	return window::MatchTemplate(this->itemArea, item->icon);
}

bool BaseInventory::CountStacks(items::Item* item, int& stacksOut, bool search)
{
	if (search && !this->Has(item)) {
		this->searchBar.SearchFor(item->name);
		Sleep(100);
	}

	auto matches = window::LocateAllTemplate(this->itemArea, item->icon, 0.9);

	if (matches.empty()) {
		stacksOut = 0;
		return true;
	}
	stacksOut = matches.size();
	return stacksOut != MAX_ITEMS_PER_PAGE;
}

const BaseInventory::Slot* BaseInventory::FindItem(
	items::Item* item, bool isSearched, bool searchFor)
{
	if (!item->hasAmbigiousQuery && (isSearched || searchFor)) {
		if (searchFor) {
			this->searchBar.SearchFor(item->name);
		}
		return this->slots[0].HasItem(item) ? &this->slots[0] : nullptr;
	}

	if (searchFor) {
		this->searchBar.SearchFor(item->name);
	}
	for (const Slot& slot : this->slots) {
		if (slot.HasItem(item)) {
			return &slot;
		}
		else if (!slot.HasItem()) {
			return nullptr;
		}
	}
	return nullptr;
}
asa::window::Rect BaseInventory::GetArea() const { return this->area; }

void BaseInventory::SetArea(const window::Point& origin)
{
	this->area = { origin.x, origin.y, 600, 832 };
}

void BaseInventory::InitSlots(const window::Point& origin)
{
	int x = origin.x + 31;
	int y = origin.y + 147;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			this->slots[(i * 6) + j] = Slot(x + (j * 93), y + (i * 93));
		}
	}
}


void BaseInventory::Popcorn(items::Item* item)
{
	int tmp{ 0 };
	this->Popcorn(item, -1, tmp);
}

void BaseInventory::Popcorn(items::Item* item, int stacks)
{
	int tmp{ 0 };
	this->Popcorn(item, stacks, tmp);
}

void BaseInventory::Popcorn(items::Item* item, int stacks, int& stacksDropped)
{
	int dropped = 0;

	if (!this->searchBar.TextIsEntered()) {
		this->searchBar.SearchFor(item->name);
	}

	while (this->slots[0].HasItem(item) && (dropped < stacks || stacks == -1)) {
		for (int i = 0; i < 4; i++) {
			window::SetMousePos(this->slots[i].GetRandLocation(5));
			Sleep(20);
			controls::KeyPress(settings::actionMappings::dropItem.key);
			Sleep(100);
			dropped++;
		}
	}

	stacksDropped = dropped;
}


void BaseInventory::PopcornSlots(int slots)
{
	for (int slot = slots - 1; slot >= 0; slot--) {
		window::SetMousePos(this->slots[slot].GetRandLocation(5));
		Sleep(20);
		controls::KeyPress(settings::actionMappings::dropItem.key);
		Sleep(100);
	}
}

void BaseInventory::TakeSlot(Slot slot)
{
	this->SelectSlot(slot);
	window::Press(settings::transferItem, false, std::chrono::milliseconds(15));
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void BaseInventory::TakeSlot(int index) { TakeSlot(this->slots[index]); }

void BaseInventory::Close()
{
	auto start = std::chrono::system_clock::now();
	while (this->IsOpen()) {
		window::Press("esc", true);
		if (util::Await([this]() { return !this->IsOpen(); },
				std::chrono::seconds(5))) {
			return;
		}

		if (util::Timedout(start, std::chrono::seconds(30))) {
			throw exceptions::InterfaceNotClosedError(this);
		}
	}
}

void BaseInventory::SelectSlot(Slot slot)
{
	window::Point location = slot.GetRandLocation(5);
	window::SetMousePos(location);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void BaseInventory::SelectSlot(int index)
{
	return this->SelectSlot(this->slots[index]);
}

void BaseInventory::DropAll()
{
	this->dropAllButton.Press();
	this->searchBar.SetTextCleared();
	Sleep(200);
	// TO DO: Wait for the items to be dropped
}

void BaseInventory::TransferAll(items::Item* item, BaseInventory* tar)
{
	if (item) {
		this->searchBar.SearchFor(item->name);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	this->transferAllButton.Press();
	this->searchBar.SetTextCleared();
	Sleep(200);
	// TO DO: Wait for the items to be transferred
}

void BaseInventory::TransferAll(const std::string& term, BaseInventory* tar)
{

	this->searchBar.SearchFor(term);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	return this->TransferAll(nullptr, tar);
}

void BaseInventory::Transfer(
	items::Item* item, int amount, BaseInventory*, bool search)
{
	if (search) {
		this->searchBar.SearchFor(item->name);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	int i = 0;
	while (auto slot = this->FindItem(item, search)) {
		if (i++ > amount && amount != 0) {
			break;
		}
		this->TakeSlot(*slot);
	}
}