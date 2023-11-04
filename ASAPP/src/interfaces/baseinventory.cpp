#include "baseinventory.h"
#include "../game/resources.h"

using namespace asa::interfaces;

[[nodiscard]] bool BaseInventory::ManagementButton::IsToggled() const
{
	window::Color toggledColor(128, 231, 255);
	auto masked = window::GetMask(this->area, toggledColor, 25);
	return cv::countNonZero(masked) > 30;
}

[[nodiscard]] bool BaseInventory::ManagementButton::IsAvailable() const
{
	window::Color baseColor(0, 140, 171);
	auto masked = window::GetMask(this->area, baseColor, 25);
	return cv::countNonZero(masked) > 20;
}

[[nodiscard]] bool BaseInventory::InvTabButton::IsSelected() const
{
	window::Color selectedColor(188, 244, 255);
	auto masked = window::GetMask(this->area, selectedColor, 25);

	cv::imshow("t", masked);
	cv::waitKey(1);

	return cv::countNonZero(masked) > 100;
}

[[nodiscard]] bool BaseInventory::InvTabButton::Exists() const
{
	window::Color inactiveColor(80, 141, 155);
	auto masked = window::GetMask(this->area, inactiveColor, 25);

	cv::imshow("t", masked);
	cv::waitKey(1);

	return cv::countNonZero(masked) > 100 || this->IsSelected();
}

bool BaseInventory::IsOpen()
{
	return window::MatchTemplate(
		this->itemFilter.area, resources::cb_arrowdown);
}

asa::window::Rect BaseInventory::GetArea() const { return this->area; }

void BaseInventory::SetArea(const window::Point& origin)
{
	this->area = { origin.x, origin.y, 600, 832 };
}