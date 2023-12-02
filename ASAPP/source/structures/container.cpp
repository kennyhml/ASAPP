#include "asapp/structures/container.h"

using namespace asa::structures;

Container::Container(
	std::string name, int slots, interfaces::BaseInventory* inv)
	: inventory(inv), maxSlots(slots),
	  InteractableStructure(
		  name, &settings::actionMappings::accessInventory, inv)
{
	if (!this->inventory) {
		this->inventory = new interfaces::BaseInventory(true);
		this->_interface = inventory;
	}
}

int Container::GetSlotCount()
{
	return this->info.GetFillLevel() * this->maxSlots;
}