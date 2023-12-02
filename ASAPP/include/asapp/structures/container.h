#pragma once
#include "../interfaces/baseinventory.h"
#include "../interfaces/containerinfo.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class Container : public InteractableStructure
	{
	public:
		Container(std::string name, interfaces::BaseInventory* inv = nullptr);

		interfaces::ContainerInfo info;
		interfaces::BaseInventory* inventory;
	};
}