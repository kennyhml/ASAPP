#pragma once
#include "container.h"
#include "../interfaces/craftinginventory.h"

namespace asa::structures
{
    class CraftingStation : public Container
    {
    public:
        CraftingStation(std::string t_name, int t_max_slots,
                        std::unique_ptr<interfaces::CraftingInventory> t_inv = nullptr,
                        std::unique_ptr<interfaces::ContainerInfo> t_info = nullptr);

        /**
         * @brief Gets the crafting inventory component of the crafting station. 
         */
        [[nodiscard]] interfaces::CraftingInventory* get_inventory() const override
        {
            return dynamic_cast<interfaces::CraftingInventory*>(interface_.get());
        }
    };
}
