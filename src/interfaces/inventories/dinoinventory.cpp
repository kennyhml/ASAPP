#include "asa/interfaces/inventories/dinoinventory.h"
#include "asa/utility.h"

namespace asa
{
    void dino_inventory::equip(item& item, const dino_info::slot slot)
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
            window::press(get_action_mapping("Use"));
        } while (!utility::await([this, &item, slot] {
            return get_info()->get_slot(slot).has(item);
        }, 5s));
    }
}
