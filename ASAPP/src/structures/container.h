#pragma once
#include "../interfaces/baseinventory.h"
#include "basestructure.h"

namespace asa::structures
{
	class Container : public BaseStructure
	{
	public:
		Container(std::string name,
			interfaces::BaseInventory* associatedInventory = nullptr)
			: inventory(associatedInventory),
			  BaseStructure(name, associatedInventory)
		{
			if (!this->inventory) {
				this->inventory = new interfaces::BaseInventory(true);
				this->_interface = inventory;
			}
		}
		interfaces::BaseInventory* inventory;
	};
}