#pragma once
#include "baseentity.h"

namespace asa::entities
{
	class DinoEnt : public BaseEntity
	{
	public:
		DinoEnt(std::string name, interfaces::BaseInventory* inv = nullptr);
		DinoEnt(const char* name, interfaces::BaseInventory* inv = nullptr);

		void Exit();
	};
}