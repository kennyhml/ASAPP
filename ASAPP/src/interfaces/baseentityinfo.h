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

		virtual const int GetDay() = 0;
		virtual const int GetTime() = 0;
		virtual const int GetTemp() = 0;
		virtual const int GetArmor() = 0;

		virtual const int GetMaxStamine() = 0;
		virtual const int GetCurrentStamine() = 0;

		virtual const int GetMaxFood() = 0;
		virtual const int GetCurrentFood() = 0;

		virtual const int GetMaxWater() = 0;
		virtual const int GetCurrentWater() = 0;

		virtual const int GetMeleeMultiplier() = 0;
		virtual const int GetTorpitidy() = 0;

		virtual const bool CanLevelUp() = 0;
	};
}