#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"

namespace asa::entities
{
	class BaseEntity
	{
	protected:
		std::string name;

	public:
		BaseEntity(
			std::string name, interfaces::BaseInventory* associatedInventory)
			: name(name), inventory(associatedInventory)
		{
			if (!inventory) {
				this->inventory = new interfaces::BaseInventory(true);
			}
		};
		~BaseEntity() { delete this->inventory; }

		interfaces::BaseInventory* inventory{ nullptr };
		interfaces::ActionWheel actionWheel;

		std::string GetName() const { return this->name; }
	};
}