#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"
#include <memory>

namespace asa::entities
{
	class BaseEntity
	{
	public:
		BaseEntity(std::string t_name,
			std::unique_ptr<interfaces::BaseInventory> t_inventory = nullptr);

		std::string get_name() const { return name; }
		virtual interfaces::BaseInventory* get_inventory() const
		{
			return inventory.get();
		}

	protected:
		std::string name;
		std::unique_ptr<interfaces::BaseInventory> inventory;
		interfaces::ActionWheel action_wheel;
	};
}