#pragma once
#include "../interfaces/baseinventory.h"


namespace asa::structures
{
	class BaseStructure
	{
	public:
		interfaces::BaseInventory* inventory;
	};

}