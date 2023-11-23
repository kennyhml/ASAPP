#pragma once
#include "../interfaces/baseinventory.h"


namespace asa::entities
{
	class BaseEntity
	{
	public:
		BaseEntity(interfaces::BaseInventory* associatedInventory)
			: inventory(associatedInventory){};

		interfaces::BaseInventory* inventory{ nullptr };
	};
}