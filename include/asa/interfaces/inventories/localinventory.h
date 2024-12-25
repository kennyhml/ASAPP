#pragma once
#include "baseinventory.h"
#include "asa/interfaces/info/playerinfo.h"

namespace asa
{
    class local_inventory final : public base_inventory
    {
    public:
        local_inventory() : base_inventory(false, std::make_unique<player_info>()) {}

        enum Tab
        {
            INVENTORY,
            COSMETICS,
            CRAFTING
        };

        /**
         * @brief Gets the info component of the local inventory.
         */
        [[nodiscard]] player_info* get_info() const override
        {
            return dynamic_cast<player_info*>(info_.get());
        }

        /**
         * @brief Opens the local inventory.
         *
         * @throws failed_to_open If opening the inventory failed.
         *
         */
        [[maybe_unused]] void open();

        /**
         * @brief Switches to the specified tab.
         * 
         * @param tab The tab to switch to.
         *
         * @throws InterfaceError Upon failur to switch to the given tab.
         */
        [[maybe_unused]] void switch_to(Tab tab);

        /**
         * @brief Equips the given item into the target slot.
         * 
         * @param item The item to equip. 
         * @param slot The slot the item will be equipped in.
         */
        [[maybe_unused]] void equip(item& item, player_info::Slot slot);

        inv_tab_button inventory_tab{177, 118, 183, 47};
        inv_tab_button cosmetics_tab{358, 120, 184, 46};
        inv_tab_button crafting_tab{542, 120, 183, 45};
    };
}
