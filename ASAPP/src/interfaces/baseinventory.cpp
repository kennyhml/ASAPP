#include "baseinventory.h"
#include "../game/resources.h"

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

bool BaseInventory::IsOpen()
{
	return window::MatchTemplate(
		this->itemFilter.area, resources::cb_arrowdown);
}

bool BaseInventory::Has(items::Item* item, bool search)
{
	if (search) {
		this->searchBar.SearchFor(item->name);
		Sleep(100);
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
			this->slots[(i * 6) + j] = window::Rect(
				x + (j * 93), y + (i * 93), 86, 87);
		}
	}
}
