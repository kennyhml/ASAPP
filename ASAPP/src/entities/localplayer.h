#pragma once
#include "../interfaces/hud.h"
#include "../interfaces/localinventory.h"
#include "../interfaces/spawnmap.h"
#include "../structures/basestructure.h"
#include "../structures/container.h"
#include "playerent.h"

namespace asa::entities
{
	class LocalPlayer : public PlayerEnt
	{
	public:
		LocalPlayer(interfaces::LocalInventory* associatedInventory)
			: inventory(associatedInventory), PlayerEnt(associatedInventory){};

		interfaces::LocalInventory* inventory;

		const bool IsAlive();
		const bool IsOutOfWater()
		{
			return interfaces::gHUD->IsPlayerOutOfWater();
		}
		const bool IsOutOfFood()
		{
			return interfaces::gHUD->IsPlayerOutOfFood();
		}
		const bool IsOverweight()
		{
			return interfaces::gHUD->IsPlayerOverweight();
		}

		const bool ReceivedItem()
		{
			return interfaces::gHUD->GotItemAdded(this->inventory->IsOpen());
		}

		const bool DepositedItem()
		{
			return interfaces::gHUD->GotItemRemoved(this->inventory->IsOpen());
		}

		const bool IsInSpawnAnimation();
		const bool IsInTravelScreen();

		const bool DepositIntoDedicatedStorage(int* depositedAmountOut);
		const bool WithdrawFromDedicatedStorage(int withdrawnAmountOut);

		void Suicide();

		void Access(entities::BaseEntity*);
		void Access(structures::Container*);
		void Access(structures::InteractableStructure*);

		void Jump() { window::Press(settings::jump); }
		void Crouch() { window::Press(settings::crouch); }
		void Prone() { window::Press(settings::prone); }
		void GetUp() { window::Press(settings::run); }

		void TurnRight(int degrees = 90) { controls::TurnDegrees(degrees, 0); }
		void TurnLeft(int degrees = 90) { controls::TurnDegrees(-degrees, 0); }
		void TurnUp(int degrees = 90) { controls::TurnDegrees(0, -degrees); }
		void TurnDown(int degrees = 90) { controls::TurnDegrees(0, degrees); }

		void WalkFoward(std::chrono::milliseconds duration);
		void WalkLeft(std::chrono::milliseconds duration);
		void WalkRight(std::chrono::milliseconds duration);
		void WalkBackwards(std::chrono::milliseconds duration);

		void Equip(items::Item* item, interfaces::PlayerInfo::Slot targetSlot);
		void Unequip(interfaces::PlayerInfo::Slot targetSlot);
	};

	inline LocalPlayer* gLocalPlayer = new LocalPlayer(
		asa::interfaces::gLocalinventory);
}