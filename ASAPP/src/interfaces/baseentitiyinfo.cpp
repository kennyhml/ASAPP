#include "../../include/asa/interfaces/info/baseentityinfo.h"
#include "../../include/asa/core/state.h"

namespace asa::interfaces
{
    namespace
    {
        float get_fill_amount(const window::Rect& bar)
        {
            static constexpr window::Color color{1, 156, 136};

            const int filled = cv::countNonZero(window::get_mask(bar, color, 20));
            return static_cast<float>(filled) / static_cast<float>(bar.width);
        }
    }

    base_entity_info::BaseEntityInfo()
    {
        for (int i = 0; i < gear_slots.max_size(); i++) {
            gear_slots[i] = GearSlot(764 + (305 * (i > 2)), 178 + (93 * (i % 3)));
        }
    }

    float base_entity_info::get_health_level()
    {
        static const window::Rect roi(764, 514, 336, 1);
        return get_fill_amount(roi);
    }

    float base_entity_info::get_food_level()
    {
        static const window::Rect roi(764, 619, 336, 1);
        return get_fill_amount(roi);
    }

    float base_entity_info::get_water_level()
    {
        static const window::Rect roi(764, 654, 336, 1);
        return get_fill_amount(roi);
    }

    void base_entity_info::unequip(const Slot slot)
    {
        const auto& gear_slot = gear_slots.at(slot);

        while (!gear_slots.empty()) {
            auto point = gear_slot.area.get_random_location(5);
            window::click_at(point, controls::LEFT);
            core::sleep_for(5ms);
            window::click_at(point, controls::LEFT);

            core::sleep_for(10ms);
        }
    }

    void base_entity_info::unequip_all()
    {
        for (int i = 0; i < gear_slots.size(); i++) {
            unequip(static_cast<Slot>(i));
        }
    }
}
