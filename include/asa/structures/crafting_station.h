#pragma once
#include "container.h"
#include "asa/interfaces/inventories/crafting_inventory.h"

namespace asa
{
    class crafting_station : public container
    {
    public:
        crafting_station(std::string t_name, int t_max_slots,
                        std::unique_ptr<crafting_inventory> t_inv = nullptr,
                        std::unique_ptr<container_info> t_info = nullptr);

        /**
         * @brief Gets the crafting inventory component of the crafting station. 
         */
        [[nodiscard]] crafting_inventory* get_inventory() const override
        {
            return dynamic_cast<crafting_inventory*>(interface_.get());
        }
    };
}
