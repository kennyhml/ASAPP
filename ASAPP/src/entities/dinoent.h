#pragma once
#include "baseentity.h"

namespace asa::entities
{

	class DinoEnt : public BaseEntity
	{

	private:
		std::string name;

	public:
		DinoEnt(
			std::string name, interfaces::BaseInventory* inventory = nullptr)
			: BaseEntity(inventory){};

		const std::string& GetName() { return this->name; }

		void Exit()
		{
			this->inventory->Close();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	};
}