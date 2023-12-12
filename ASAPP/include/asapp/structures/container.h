#pragma once
#include "../interfaces/baseinventory.h"
#include "../interfaces/containerinfo.h"
#include "interactablestructure.h"

namespace asa::structures
{
	class Container : public InteractableStructure
	{
	protected:
		int max_slots;

	public:
		Container(std::string t_name, int t_slots,
			interfaces::BaseInventory* inv = nullptr);

		interfaces::ContainerInfo info;
		interfaces::BaseInventory* inventory;

		int get_slot_count();
		int get_max_slots() const { return max_slots; }
	};

}