#pragma once
#include "baseinfo.h"
#include "asa/interfaces/components/button.h"
#include "asa/interfaces/components/slot.h"

#include <array>

namespace asa
{
    class base_entity_info : public base_info
    {
    protected:
        struct gear_slot : item_slot
        {
            using item_slot::item_slot;

            cv::Rect get_slot_description_area() const
            {
                return {area.x, area.y + 68, 87, 17};
            }
        };

    public:
        base_entity_info();

        enum slot : uint32_t;

        enum Stat : uint32_t
        {
            HEALTH,
            STAMINA,
            FOOD,
            WEIGHT,
            MELEE,
            TORPIDITY,
        };

        virtual const gear_slot& get_slot(const int slot) const
        {
            return gear_slots_.at(slot);
        };

        /**
         * @brief Unequips the item in a given slot.
         */
        void unequip(slot slot);

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
        std::array<gear_slot, 6> gear_slots_;
        cv::Rect info_area_{859, 177, 200, 275};
        button toggle_buffs_button_{765, 866, 392, 36};
    };
}
