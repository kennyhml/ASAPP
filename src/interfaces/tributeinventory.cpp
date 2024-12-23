#include "asa/interfaces/tributeinventory.h"

namespace asa::interfaces
{
    TributeInventory::TributeInventory() : BaseInventory(true)
    {
        // Adjust for the search bar being bigger in tribute inventories for some reason.
        // We can leave the search bar as is, it doesnt matter that its bigger than usual.
        transfer_all_button_ = ManagementButton(1404, 176);
    }

    void TributeInventory::summon(const std::string& boss_name)
    {
        search_bar.search_for(boss_name);
        select_slot(0);
        controls::press(settings::use);
    }
}
