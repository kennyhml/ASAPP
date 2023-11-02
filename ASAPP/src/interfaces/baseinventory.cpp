#include "baseinventory.h"
#include "../game/resources.h"

bool asa::BaseInventory::IsOpen()
{

	return window::MatchTemplate(
		this->itemFilter.area, resources::cb_arrowdown);
}

asa::window::Rect asa::BaseInventory::GetArea() const { return this->area; }

void asa::BaseInventory::SetArea(const window::Point& origin)
{
	this->area = { origin.x, origin.y, 600, 832 };
}