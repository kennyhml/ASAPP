#pragma once
#include "baseinventory.h"
#include "playerinfo.h"

namespace asa::interfaces
{
    class LocalInventory final : public BaseInventory
    {
    public:
        LocalInventory() : BaseInventory(false) {}

        enum Tab
        {
            INVENTORY,
            COSMETICS,
            CRAFTING
        };

        PlayerInfo info{};

        InvTabButton inventory_tab{177, 118, 183, 47};
        InvTabButton cosmetics_tab{358, 120, 184, 46};
        InvTabButton crafting_tab{542, 120, 183, 45};

        /**
         * @brief Opens the local inventory.
         *
         * @throws InventoryNotOpenedError If opening the inventory failed.
         */
        void open();
        /**
         * @brief Switches to the specified tab.
         * 
         * @param tab The tab to switch to.
         *
         * @throws InterfaceError Upon failur to switch to the given tab.
         */
        void switch_to(Tab tab);

        /**
         * @brief Toggles item tooltips on/off.
         */
        void toggle_tooltips();

        /**
         * @brief Equips the given item into the target slot.
         * 
         * @param item The item to equip. 
         * @param slot The slot the item will be equipped in.
         */
        void equip(items::Item& item, const PlayerInfo::Slot slot);
    };

    inline std::unique_ptr<LocalInventory> local_inventory = std::make_unique<
        LocalInventory>();
}
