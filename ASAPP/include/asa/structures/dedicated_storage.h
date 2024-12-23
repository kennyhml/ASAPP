#pragma once
#include "container.h"
#include "asa/interfaces/inventories/baseinventory.h"
#include "asa/interfaces/info/dedicated_storage_info.h"

namespace asa
{
    class dedicated_storage final : public container
    {
    public:
        explicit dedicated_storage(std::string name)
            : container(
                std::move(name), 1800, nullptr,
                std::make_unique<dedicated_storage_info>()) {}

        /**
         * @brief Checks whether depositing into the dedicated storage is possible.
         * 
         * @return True if we can deposit items into the dedi, false otherwise.
         */
        [[nodiscard]] bool can_deposit() const;

        /**
         * @brief Gets the info interface component of the dedicated storage. 
         *
         * @remark This is a special component that allows item withdrawal.
         */
        [[nodiscard]] dedicated_storage_info* get_info() const override
        {
            return dynamic_cast<dedicated_storage_info*>(info_.get());
        }
    };
}
