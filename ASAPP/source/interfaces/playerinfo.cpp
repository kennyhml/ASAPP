#include "asapp/interfaces/playerinfo.h"
#include <stdexcept>
#include "asapp/core/state.h"
#include "asapp/game/globals.h"

namespace asa::interfaces
{
    bool PlayerInfo::is_open() const
    {
        window::Color textColor(188, 244, 255);
        auto mask = get_mask(you.area, textColor, 20);
        return countNonZero(mask) > 60;
    }

    const bool PlayerInfo::has_equipped(items::Item* item, Slot slot)
    {
        if (item && (!item->get_data().type == item->get_data().EQUIPPABLE)) {
            throw std::invalid_argument(
                std::format("Item '{}' cannot be equipped.", item->get_name()));
        }

        GearSlot gear_slot = gear_slots[slot];
        const auto roi = item ? gear_slot : gear_slot.get_slot_description_area();

        if (!item) {
            auto mask = get_mask(roi, window::Color(223, 250, 255), 20);
            return countNonZero(mask) < 30;
        }
        return match_template(roi, item->get_inventory_icon(), 0.7,
                              item->get_inventory_icon_mask());
    }

    void PlayerInfo::unequip(Slot slot)
    {
        const auto& gear_slot = gear_slots[slot];
        while (has_equipped(nullptr, slot)) {
            auto point = gear_slot.get_random_location(5);
            if (globals::useWindowInput) {
                click_at(point, controls::LEFT);
                core::sleep_for(std::chrono::milliseconds(5));
                click_at(point, controls::LEFT);
            }
            else {
                set_mouse_pos(point);
                core::sleep_for(std::chrono::milliseconds(15));
                for (int i = 0; i < 3; i++) { controls::press(settings::use); }
            }
            core::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void PlayerInfo::unequip_all()
    {
        bool any_let = true;
        while (any_let) {
            any_let = false;

            for (const auto& slot : gear_slots) {
                auto point = slot.get_random_location(5);
                if (globals::useWindowInput) {
                    click_at(slot.get_random_location(5), controls::LEFT);
                    core::sleep_for(std::chrono::milliseconds(5));
                    click_at(slot.get_random_location(5), controls::LEFT);
                }
                else {
                    set_mouse_pos(point);
                    core::sleep_for(std::chrono::milliseconds(15));
                    for (int i = 0; i < 3; i++) { controls::press(settings::use); }
                }
                core::sleep_for(std::chrono::milliseconds(100));
            }
            core::sleep_for(std::chrono::milliseconds(1000));
            int i = 0;
            any_let = std::any_of(gear_slots.begin(), gear_slots.end(),
                                  [&i, this](GearSlot slot) {
                                      return has_equipped(
                                          nullptr, static_cast<Slot>(i++));
                                  });
        }
    }

    const items::Item* PlayerInfo::get_equipped_item(Slot slot) { return nullptr; }
}
