#include "asapp/structures/container.h"

using namespace asa::structures;

Container::Container(
	std::string name, interfaces::BaseInventory* associatedInventory)
	: inventory(associatedInventory),
	  InteractableStructure(
		  name, &settings::actionMappings::accessInventory, associatedInventory)
{
	if (!this->inventory) {
		this->inventory = new interfaces::BaseInventory(true);
		this->_interface = inventory;
	}
}