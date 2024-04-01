#pragma once
#include "baseinfo.h"
#include "components/button.h"
#include "components/slot.h"

#include <array>


namespace asa::interfaces
{
    class BaseEntityInfo : public BaseInfo
    {
    protected:
        struct GearSlot : components::Slot
        {
            using Slot::Slot;

            window::Rect get_slot_description_area() const
            {
                return {area.x, area.y + 68, 87, 17};
            }
        };

    public:
        BaseEntityInfo();

        enum Slot : uint32_t;

        enum Stat : uint32_t
        {
            HEALTH,
            STAMINA,
            FOOD,
            WEIGHT,
            MELEE,
            TORPIDITY,
        };

        virtual const GearSlot& get_slot(const int slot) const
        {
            return gear_slots.at(slot);
        };

        /**
         * @brief Unequips the item in a given slot.
         */
        void unequip(Slot slot);

        /**
         * @brief Unequips all items.
         */
        void unequip_all();

        virtual int get_level() { return 0; }

        virtual int get_day() { return 0; }
        virtual int get_time() { return 0; }
        virtual int get_temp() { return 0; }
        virtual int get_total_armor() { return 0; }

        int get_max_health() override { return 0; }
        int get_current_health() override { return 0; }

        float get_health_level() override;

        int get_max_weight() override { return 0; }
        int get_current_weight() override { return 0; }

        std::string get_owner() override { return ""; }
        std::string get_name() override { return ""; }

        virtual int get_max_stamina() { return 0; }
        virtual int get_current_stamina() { return 0; }

        virtual int get_max_food() { return 0; }
        virtual int get_current_food() { return 0; }

        virtual float get_food_level();

        virtual int get_max_water() { return 0; }
        virtual int get_current_water() { return 0; }

        virtual float get_water_level();

        virtual int get_melee_multiplier() { return 0; }
        virtual int get_torpitidy() { return 0; }

        virtual bool can_level_up() { return false; }

    protected:
        std::array<GearSlot, 6> gear_slots;
        window::Rect info_area{859, 177, 200, 275};
        components::Button toggle_buffs_button{765, 866, 392, 36};
    };
}
