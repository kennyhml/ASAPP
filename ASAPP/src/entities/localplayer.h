#pragma once
#include "../interfaces/localinventory.h"
#include "playerent.h"

namespace asa::entities
{
	class LocalPlayer : public PlayerEnt
	{
	public:
		LocalPlayer(interfaces::LocalInventory* associatedInventory)
			: inventory(associatedInventory), PlayerEnt(associatedInventory){};

		interfaces::LocalInventory* inventory{ nullptr };

		[[nodiscard]] const bool IsAlive();
		[[nodiscard]] const bool IsOverweight();
		[[nodiscard]] const bool IsBrokenBones();
		[[nodiscard]] const bool IsOutOfWater();
		[[nodiscard]] const bool IsOutOfFood();
		[[nodiscard]] const bool IsSprinting();

		void Jump();
		void WalkFoward(std::chrono::milliseconds duration);
		void WalkLeft(std::chrono::milliseconds duration);
		void WalkRight(std::chrono::milliseconds duration);
		void WalkBackwards(std::chrono::milliseconds duration);

		void Equip(items::Item* item, interfaces::PlayerInfo::Slot targetSlot);
	};
}