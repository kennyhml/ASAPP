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
		const bool CanAccessBed();
		const bool CanUseDefaultTeleport();

		const bool DepositIntoDedicatedStorage(int* depositedAmountOut);
		const bool WithdrawFromDedicatedStorage(int* withdrawnAmountOut);

		void Suicide();

		const bool CanAccess(const structures::BaseStructure&);
		const bool CanAccess(const entities::BaseEntity&);

		void Access(const entities::BaseEntity&);
		void Access(const structures::Container&);
		void Access(const structures::InteractableStructure&);

		void FastTravelTo(const structures::SimpleBed&);
		void TeleportTo(const structures::Teleporter&, bool isDefault = false);
		void LayOn(const structures::SimpleBed&);
		void GetOffBed();

		void Jump() { window::Press(settings::jump); }
		void Crouch() { window::Press(settings::crouch); }
		void Prone() { window::Press(settings::prone); }
		void StandUp() { window::Press(settings::run); }

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

		void LookAllTheWayDown();
		void LookAllTheWayUp();

	private:
		void PassTravelScreen(bool in = true, bool out = true);
		void PassTeleportScreen();
	};

	inline LocalPlayer* gLocalPlayer = new LocalPlayer(
		asa::interfaces::gLocalinventory);
}