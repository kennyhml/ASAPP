#include "asapp/interfaces/localinventory.h"
#include "asapp/util/util.h"
#include "asapp/game/controls.h"
#include "asapp/game/settings.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
    void LocalInventory::open()
    {
        auto start = std::chrono::system_clock::now();
        while (!is_open()) {
            window::press(settings::show_my_inventory, true);
            if (util::await([this]() { return is_open(); }, std::chrono::seconds(5))) {
                break;
            }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotOpenedError(this);
            }
        }
    }

    void LocalInventory::switch_to(Tab tab)
    {
        InvTabButton* button = nullptr;
        switch (tab) {
        case INVENTORY: button = &inventory_tab;
            break;
        case COSMETICS: button = &cosmetics_tab;
            break;
        case CRAFTING: button = &crafting_tab;
            break;
        }
        assert(button != nullptr);

        auto start = std::chrono::system_clock::now();
        while (!button->is_selected()) {
            button->press();
            if (util::await([button]() { return button->is_selected(); },
                            std::chrono::seconds(5))) { return; }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceError(this, "Failed to open tab " + std::to_string(tab));
            }
        }
    }

    void LocalInventory::equip(items::Item* item, PlayerInfo::Slot slot)
    {
        bool searched = false;
        if (!has(item, false)) {
            searched = true;
            if (!has(item, true)) {
                throw std::runtime_error(std::format(
                    "No '{}' in local player inventory", item->get_name()));
            }
        }

        const components::Slot* itemLocation = find_item(item, searched);
        select_slot(*itemLocation);

        do { window::press(settings::action_mappings::use); }
        while (!util::await(
            [this, item, slot]() { return info.has_equipped(item, slot); },
            std::chrono::seconds(5)));
    }
}
