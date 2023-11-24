#pragma once
#include "../interfaces/baseinventory.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class Container : public InteractableStructure
	{
	public:
		Container(std::string name,
			interfaces::BaseInventory* associatedInventory = nullptr)
			: inventory(associatedInventory),
			  InteractableStructure(name,
				  &settings::actionMappings::accessInventory,
				  associatedInventory)
		{
			if (!this->inventory) {
				this->inventory = new interfaces::BaseInventory(true);
				this->_interface = inventory;
			}
		}
		interfaces::BaseInventory* inventory;
	};
}