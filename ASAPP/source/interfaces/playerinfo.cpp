#include "asapp/interfaces/playerinfo.h"
#include <iostream>
#include "asapp/core/state.h"

namespace asa::interfaces
{
    bool PlayerInfo::is_open() const
    {
        static constexpr window::Color text_color{188, 244, 255};
        return cv::countNonZero(get_mask(you_.area, text_color, 20)) > 60;
    }

    bool PlayerInfo::has_equipped(items::Item& item, const Slot slot) const
    {
        if (item.get_data().type != items::ItemData::EQUIPPABLE) {
            std::cerr << std::format(
                "[!] WARNING: {} called with bad category item: '{}'\n", __func__,
                item.get_name());
            return false;
        }
        return gear_slots[slot].has(item);
    }

    bool PlayerInfo::slot_empty(const Slot slot) const
    {
        static constexpr window::Color slot_title_color{223, 250, 255};
        const auto roi = gear_slots[slot].get_slot_description_area();
        return cv::countNonZero(get_mask(roi, slot_title_color, 20)) > 30;
    }

    void PlayerInfo::unequip(const Slot slot)
    {
        const auto& gear_slot = gear_slots[slot];
        while (!slot_empty(slot)) {
            auto point = gear_slot.area.get_random_location(5);
            window::click_at(point, controls::LEFT);
            core::sleep_for(std::chrono::milliseconds(5));
            window::click_at(point, controls::LEFT);

            core::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void PlayerInfo::unequip_all() {}

    std::unique_ptr<items::Item> PlayerInfo::get_equipped_item(const Slot slot) const
    {
        return gear_slots[slot].get_item();
    }
}
