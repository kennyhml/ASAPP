#pragma once
#include "asapp/items/items.h"
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

		virtual bool is_open() const;

		const int get_tame_limit() { return 0; }
		const int get_crafting_skill() { return 0; }
		const int get_fortitude() { return 0; }

		const bool get_gear_durability(Slot slot, int& duraOut) { return 0; }
		const bool get_gear_armor(Slot slot, int& armorOut) { return 0; }
		const bool has_equipped(items::Item*, Slot slot);

		void unequip(Slot slot);
		void unequip_all();

		const items::Item* get_equipped_item(Slot slot);
	};






}