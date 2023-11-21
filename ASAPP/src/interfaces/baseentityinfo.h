#pragma once
#include "baseinfo.h"
#include "components/button.h"
#include <array>

namespace asa::interfaces
{

	class BaseEntityInfo : public BaseInfo
	{

	protected:
		struct GearSlot : public window::Rect
		{
			GearSlot() : Rect{ 0, 0, 87, 87 } {}
			GearSlot(int x, int y) : Rect{ x, y, 87, 87 } {};
		};

		std::array<GearSlot, 6> gearSlots;
		window::Rect infoArea{ 859, 177, 200, 275 };
		components::Button toggleBuffsButton{ 765, 866, 392, 36 };

	public:
		BaseEntityInfo()
		{
			for (int i = 0; i < gearSlots.max_size(); i++) {
				gearSlots[i] = GearSlot(
					764 + (305 * i > 2), 178 + (93 * (i % 3)));
			}
		}

		enum Stat
		{
			HEALTH,
			STAMINA,
			FOOD,
			WEIGHT,
			MELEE,
			TORPIDITY,
		};

		virtual const int GetLevel() { return 0; }

		virtual const int GetDay() { return 0; }
		virtual const int GetTime() { return 0; }
		virtual const int GetTemp() { return 0; }
		virtual const int GetTotalArmor() { return 0; }

		virtual const int GetMaxHealth() { return 0; }
		virtual const int GetCurrentHealth() { return 0; }

		virtual const int GetMaxWeight() { return 0; };
		virtual const int GetCurrentWeight() { return 0; };

		virtual const std::string GetOwner() { return ""; }
		virtual const std::string GetName() { return ""; }

		virtual const int GetMaxStamine() { return 0; }
		virtual const int GetCurrentStamine() { return 0; }

		virtual const int GetMaxFood() { return 0; }
		virtual const int GetCurrentFood() { return 0; }

		virtual const int GetMaxWater() { return 0; }
		virtual const int GetCurrentWater() { return 0; }

		virtual const int GetMeleeMultiplier() { return 0; }
		virtual const int GetTorpitidy() { return 0; }

		virtual const bool CanLevelUp() { return 0; }
	};
}