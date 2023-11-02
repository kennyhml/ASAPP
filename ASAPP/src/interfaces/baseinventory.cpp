#include "baseinventory.h"

bool asa::BaseInventory::IsOpen()
{
	auto img = cv::imread(
		"src/assets/interfaces/cb_arrowdown.png", cv::IMREAD_COLOR);

	return window::MatchTemplate(this->itemFilter.area, img);
}

asa::window::Rect asa::BaseInventory::GetArea() const { return this->area; }

void asa::BaseInventory::SetArea(const window::Point& origin)
{
	this->area = { origin.x, origin.y, 600, 832 };
}