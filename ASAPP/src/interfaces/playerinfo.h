#pragma once
#include "../items/items.h"
#include "baseentityinfo.h"

namespace asa::interfaces
{
	class PlayerInfo : public BaseEntityInfo
	{
	protected:
		components::Button you{ 757, 124, 111, 49 };

	public:
		using BaseEntityInfo::BaseEntityInfo;
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
			WATER,
			WEIGHT,
			MELEE,
			CRAFTING_SKILL,
			FORTITUDE,
			TORPIDITY
		};

		bool IsOpen();

		const int GetTameLimit() { return 0; }
		const int GetCraftingSkill() { return 0; }
		const int GetFortitude() { return 0; }

		const bool GetGearDurability(Slot slot, int& duraOut) { return 0; }
		const bool GetGearArmor(Slot slot, int& armorOut) { return 0; }
		const bool HasEquipped(const items::Item*, Slot slot);

		void Unequip(Slot slot);
		void UnequipAll();

		const items::Item* GetEquippedItem(Slot slot);
	};






}