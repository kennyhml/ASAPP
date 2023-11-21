#pragma once
#include "../items/items.h"
#include "baseentityinfo.h"

namespace asa::interfaces
{

	class PlayerInfo : public BaseEntityInfo
	{
		using BaseEntityInfo::BaseEntityInfo;


	public:
		enum Slot
		{
			HEAD,
			TORSO,
			LEGS,
			HANDS,
			OFFHAND,
			FEET,
		};

		enum Stat
		{
			HEALTH,
			FOOD,
			OXYGEN,
			FOOD,
			WATER,
			WEIGHT,
			MELEE,
			CRAFTING_SKILL,
			FORTITUDE,
			TORPIDITY
		};

		const int GetTameLimit();
		const int GetTotalArmor();
		const int GetColdResist();
		const int GetHeatResist();

		const bool HasEquipped(const items::Item*, Slot slot);
		const bool GetGearDurability(Slot slot, int& durabilityOut);
		const bool GetGearArmor(Slot slot, int& armorOut);

		const int GetCraftingSkill();
		const int GetFortitude();

		void UnequipItemAt(Slot slot);
		const items::Item* GetEquippedItem(Slot slot);
	};






}