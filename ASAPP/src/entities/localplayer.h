#pragma once
#include "../interfaces/hud.h"
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
		interfaces::HUD* hud = interfaces::gHUD;

		const bool IsAlive();
		const bool IsOutOfWater() { return this->hud->IsPlayerOutOfWater(); }
		const bool IsOutOfFood() { return this->hud->IsPlayerOutOfFood(); }
		const bool IsOverweight() { return this->hud->IsPlayerOverweight(); }

		void Jump() { window::Press(settings::jump); }
		void Crouch() { window::Press(settings::crouch); }
		void Prone() { window::Press(settings::prone); }
		void GetUp() { window::Press(settings::run); }

		void TurnRight(int degrees = 90) { controls::TurnDegrees(degrees, 0); }
		void TurnLeft(int degrees = 90) { controls::TurnDegrees(-degrees, 0); }
		void TurnUp(int degrees = 90) { controls::TurnDegrees(0, degrees); }
		void TurnDown(int degrees = 90) { controls::TurnDegrees(0, -degrees); }

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