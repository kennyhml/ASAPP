#include "asa/interfaces/dinoinventory.h"

#include "../../include/asa/utility.h"

namespace asa::interfaces
{
    void DinoInventory::equip(items::Item& item, const dino_info::Slot slot)
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
