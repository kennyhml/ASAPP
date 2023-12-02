#pragma once
#include "baseentity.h"

namespace asa::entities
{
	class DinoEnt : public BaseEntity
	{
	public:
		DinoEnt(
			std::string name, interfaces::BaseInventory* inventory = nullptr)
			: BaseEntity(name, inventory){};

		void Exit();
	};
}