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
			: inventory(associatedInventory),
			  PlayerEnt("You", associatedInventory){};

		interfaces::LocalInventory* inventory;

		[[nodiscard]] bool IsAlive();
		[[nodiscard]] bool IsOutOfWater();
		[[nodiscard]] bool IsOutOfFood();
		[[nodiscard]] bool IsOverweight();
		[[nodiscard]] bool ReceivedItem();
		[[nodiscard]] bool DepositedItem();
		[[nodiscard]] bool IsInSpawnAnimation();
		[[nodiscard]] bool IsInTravelScreen();
		[[nodiscard]] bool CanAccessBed();
		[[nodiscard]] bool CanAccessInventory();
		[[nodiscard]] bool CanUseDefaultTeleport();

		bool DepositIntoDedicatedStorage(int* depositedAmountOut);
		bool WithdrawFromDedicatedStorage(int* withdrawnAmountOut);

		void Suicide();

		bool CanAccess(const structures::BaseStructure&);
		bool CanAccess(const entities::BaseEntity&);

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
		void PassTeleportScreen(bool allowAccessFlag = false);
	};

	inline LocalPlayer* gLocalPlayer = new LocalPlayer(
		asa::interfaces::gLocalinventory);
}