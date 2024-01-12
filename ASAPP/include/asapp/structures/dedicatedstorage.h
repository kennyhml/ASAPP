#pragma once
#include "container.h"
#include "../interfaces/baseinventory.h"
#include "../interfaces/dedicatedstorageinfo.h"

namespace asa::structures
{
    class DedicatedStorage final : public Container
    {
    public:
        explicit DedicatedStorage(std::string name) : Container(
            std::move(name), 1800, nullptr,
            std::make_unique<interfaces::DedicatedStorageInfo>()) {}

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
        [[nodiscard]] interfaces::DedicatedStorageInfo* get_info() const override
        {
            return dynamic_cast<interfaces::DedicatedStorageInfo*>(info_.get());
        }

    private:
    };
}
