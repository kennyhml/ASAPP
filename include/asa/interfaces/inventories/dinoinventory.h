#pragma once
#include "baseinventory.h"
#include "asa/interfaces/info/dino_info.h"

namespace asa
{
    class dino_inventory : public base_inventory
    {
    public:
        dino_inventory() : base_inventory(true, std::make_unique<dino_info>()) {}

        /**
         * @brief Gets the info component of the local inventory.
         */
        [[nodiscard]] dino_info* get_info() const override
        {
            return dynamic_cast<dino_info*>(info_.get());
        }

        /**
         * @brief Equips the given item into the target slot.
         *
         * @param item The item object to equip. The first result is used.
         * @param slot The slot the item will be equipped in.
         */
        void equip(item& item, dino_info::slot slot);
    };
}
