#include "asa/ui/storage/localinventory.h"
#include "asa/utility.h"
#include "asa/core/logging.h"
#include "asa/game/settings.h"
#include "asa/ui/exceptions.h"

namespace asa
{
    local_inventory& local_inventory::open()
    {
        const auto start = std::chrono::system_clock::now();
        get_logger()->trace("Opening local player inventory..");
        while (!is_open()) {
            post_press(get_action_mapping("ShowMyInventory"));
            if (utility::await([this] { return is_open(); }, 5s)) { break; }
            if (utility::timedout(start, 30s)) { throw failed_to_open(this); }
        }
        return *this;
    }

    local_inventory& local_inventory::switch_to(const Tab tab)
    {
        assert_open(__func__);

        inv_tab_button* button = nullptr;
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
            if (utility::await([button] { return button->is_selected(); }, 5s)) {
                break;
            }

            if (utility::timedout(start, std::chrono::seconds(30))) {
                throw interface_error(this, "Failed to open tab " + std::to_string(tab));
            }
        }
        return *this;
    }

    local_inventory& local_inventory::equip(item& item, const player_info::Slot slot)
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

        select_info_tab();
        do {
            post_press(get_action_mapping("Use"));
        } while (!utility::await([this, &item, slot] {
            return get_info()->get_slot(slot).has(item);
        }, 5s));
        return *this;
    }
}
