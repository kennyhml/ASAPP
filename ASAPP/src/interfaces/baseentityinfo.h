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

		virtual const int GetDay();
		virtual const int GetTime();
		virtual const int GetTemp();
		virtual const int GetArmor();

		virtual const int GetMaxStamine();
		virtual const int GetCurrentStamine();

		virtual const int GetMaxFood();
		virtual const int GetCurrentFood();

		virtual const int GetMaxWater();
		virtual const int GetCurrentWater();

		virtual const int GetMeleeMultiplier();
		virtual const int GetTorpitidy();

		virtual const bool CanLevelUp();
	};
}