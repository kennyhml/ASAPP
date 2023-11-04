#pragma once
#include "baseinventory.h"

namespace asa::interfaces
{

	class LocalInventory : BaseInventory
	{
	public:
		LocalInventory() : BaseInventory(false){};

		enum Tab
		{
			INVENTORY,
			COSMETICS,
			CRAFTING
		};

		InvTabButton inventoryTab{ 177, 118, 183, 47 };
		InvTabButton cosmeticsTab{ 358, 120, 184, 46 };
		InvTabButton craftingTab{ 542, 120, 183, 45 };

		bool Open();

		void SwitchTo(Tab tab);
	};
}