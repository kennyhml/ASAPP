#pragma once
#include "container.h"
#include "asa/interfaces/inventories/tribute_inventory.h"

namespace asa
{
    class tributable : public container
    {
    public:
        tributable(std::string t_name, int t_max_slots,
                   std::unique_ptr<tribute_inventory> t_inv = nullptr,
                   std::unique_ptr<container_info> t_info = nullptr);

        /**
         * @brief Gets the inventory component of the Tributeable Structure.
         */
        [[nodiscard]] tribute_inventory* get_inventory() const override
        {
            return dynamic_cast<tribute_inventory*>(interface_.get());
        }
    };
}
