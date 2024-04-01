#include "asapp/interfaces/localinventory.h"
#include "asapp/util/util.h"
#include "asapp/game/settings.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
    void LocalInventory::open()
    {
        const auto start = std::chrono::system_clock::now();
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

    void LocalInventory::switch_to(const Tab tab)
    {
        assert_open(__func__);

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

        const auto start = std::chrono::system_clock::now();
        while (!button->is_selected()) {
            button->press();
            if (util::await([button]() { return button->is_selected(); },
                            std::chrono::seconds(5))) { return; }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceError(this, "Failed to open tab " + std::to_string(tab));
            }
        }
    }

    void LocalInventory::equip(items::Item& item, const PlayerInfo::Slot slot)
    {
        assert_open(__func__);
        search_bar.search_for(item.get_name());

        // If the item name is unique, we can search & check the first slot only.
        if (!item.get_data().has_ambiguous_query) {
            for (const auto& s: slots) {
                if (s.is_empty() && !s.is_folder()) {
                    throw std::exception("Attempted to equip an item we do not own");
                }
                select_slot(s);
                break;
            }
        } else if (const auto pos = find_item(item, true); pos != nullptr) {
            select_slot(*pos);
        } else {
            throw std::exception("Attempted to equip an item we do not own");
        }

        do {
            window::press(settings::action_mappings::use);
        } while (!util::await([this, &item, slot]() {
            return get_info()->get_slot(slot).has(item);
        }, 5s));
    }
}
