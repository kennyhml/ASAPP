#pragma once
#include "baseinventory.h"
#include "dinoinfo.h"


namespace asa::interfaces
{
    class DinoInventory : public BaseInventory
    {
    public:
        DinoInventory() : BaseInventory(true, std::make_unique<DinoInfo>()) {}

        /**
         * @brief Gets the info component of the local inventory.
         */
        [[nodiscard]] DinoInfo* get_info() const override
        {
            return dynamic_cast<DinoInfo*>(info_.get());
        }

        /**
         * @brief Equips the given item into the target slot.
         *
         * @param item The item object to equip. The first result is used.
         * @param slot The slot the item will be equipped in.
         */
        void equip(items::Item& item, DinoInfo::Slot slot);
    };
}
