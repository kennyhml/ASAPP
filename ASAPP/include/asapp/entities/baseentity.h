#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"

namespace asa::entities
{
	class BaseEntity
	{
	public:
		BaseEntity(interfaces::BaseInventory* associatedInventory)
			: inventory(associatedInventory)
		{
			if (!inventory) {
				this->inventory = new interfaces::BaseInventory(true);
			}
		};
		~BaseEntity() { delete this->inventory; }

		interfaces::BaseInventory* inventory{ nullptr };
		interfaces::ActionWheel actionWheel;
	};
}