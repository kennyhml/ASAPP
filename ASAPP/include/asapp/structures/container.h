#pragma once
#include "../interfaces/baseinventory.h"
#include "../interfaces/containerinfo.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class Container : public InteractableStructure
	{
	protected:
		int maxSlots;

	public:
		Container(std::string name, int slots,
			interfaces::BaseInventory* inv = nullptr);

		interfaces::ContainerInfo info;
		interfaces::BaseInventory* inventory;

		int GetSlotCount();
	};

}