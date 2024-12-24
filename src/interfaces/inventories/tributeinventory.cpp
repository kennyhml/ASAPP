#include "asa/interfaces/inventories/tribute_inventory.h"

namespace asa
{
    tribute_inventory::tribute_inventory() : base_inventory(true)
    {
        // Adjust for the search bar being bigger in tribute inventories for some reason.
        // We can leave the search bar as is, it doesnt matter that its bigger than usual.
        transfer_all_button_ = management_button(1404, 176);
    }

    void tribute_inventory::summon(const std::string& boss_name)
    {
        search_bar.search_for(boss_name);
        select_slot(0);
        controls::press(get_action_mapping("Use"));
    }
}
