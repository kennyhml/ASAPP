#pragma once
#include "asapp/interfaces/localinventory.h"
#include "asapp/interfaces/spawnmap.h"
#include "asapp/structures/basestructure.h"
#include "asapp/structures/container.h"
#include "asapp/structures/simplebed.h"
#include "asapp/structures/teleporter.h"
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
		const bool IsOutOfWater();
		const bool IsOutOfFood();
		const bool IsOverweight();
		const bool ReceivedItem();
		const bool DepositedItem();
		const bool IsInSpawnAnimation();
		const bool IsInTravelScreen();

		const bool DepositIntoDedicatedStorage(int* depositedAmountOut);
		const bool WithdrawFromDedicatedStorage(int* withdrawnAmountOut);

		void Suicide();

		void Access(entities::BaseEntity*);
		void Access(structures::Container*);
		void Access(structures::InteractableStructure*);

		void FastTravelTo(structures::SimpleBed*);
		void TeleportTo(structures::Teleporter*, bool isDefault = false);

		void Jump() { window::Press(settings::jump); }
		void Crouch() { window::Press(settings::crouch); }
		void Prone() { window::Press(settings::prone); }
		void GetUp() { window::Press(settings::run); }

		void TurnRight(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void TurnLeft(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void TurnUp(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void TurnDown(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));

		void WalkFoward(std::chrono::milliseconds duration);
		void WalkLeft(std::chrono::milliseconds duration);
		void WalkRight(std::chrono::milliseconds duration);
		void WalkBackwards(std::chrono::milliseconds duration);

		void Equip(items::Item* item, interfaces::PlayerInfo::Slot targetSlot);
		void Unequip(interfaces::PlayerInfo::Slot targetSlot);

	private:
		void PassTravelScreen();
		void PassTeleportScreen();
		void LookDown();
	};

	inline LocalPlayer* gLocalPlayer = new LocalPlayer(
		asa::interfaces::gLocalinventory);
}