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
			GearSlot(int t_x, int t_y) : Rect{ t_x, t_y, 87, 87 } {};

			window::Rect get_slot_description_area()
			{
				return window::Rect(x, y + 68, 87, 17);
			}
		};

		std::array<GearSlot, 6> gear_slots;
		window::Rect info_area{ 859, 177, 200, 275 };
		components::Button toggle_buffs_button{ 765, 866, 392, 36 };

	public:
		BaseEntityInfo()
		{
			for (int i = 0; i < gear_slots.max_size(); i++) {
				gear_slots[i] = GearSlot(
					764 + (305 * (i > 2)), 178 + (93 * (i % 3)));
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

		virtual int get_level() { return 0; }

		virtual int get_day() { return 0; }
		virtual int get_time() { return 0; }
		virtual int get_temp() { return 0; }
		virtual int get_total_armor() { return 0; }

		virtual int get_max_health() { return 0; }
		virtual int get_current_health() { return 0; }

		virtual int get_max_weight() { return 0; };
		virtual int get_current_weight() { return 0; };

		virtual std::string get_owner() { return ""; }
		virtual std::string get_name() { return ""; }

		virtual int get_max_stamine() { return 0; }
		virtual int get_current_stamine() { return 0; }

		virtual int get_max_food() { return 0; }
		virtual int get_current_food() { return 0; }

		virtual int get_max_water() { return 0; }
		virtual int get_current_water() { return 0; }

		virtual int get_melee_multiplier() { return 0; }
		virtual int get_torpitidy() { return 0; }

		virtual bool can_level_up() { return 0; }
	};
}