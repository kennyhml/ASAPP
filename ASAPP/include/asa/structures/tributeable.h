#pragma once
#include "container.h"
#include "asa/interfaces/tributeinventory.h"

namespace asa::structures
{
    class Tributable : public Container
    {
    public:
        Tributable(std::string t_name, int t_max_slots,
                  std::unique_ptr<interfaces::TributeInventory> t_inv = nullptr,
                  std::unique_ptr<interfaces::container_info> t_info = nullptr);

        /**
         * @brief Gets the inventory component of the Tributeable Structure.
         */
        [[nodiscard]] interfaces::TributeInventory* get_inventory() const override
        {
            return dynamic_cast<interfaces::TributeInventory*>(interface_.get());
        }
    };
}
