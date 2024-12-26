#pragma once
#include "container.h"
#include "asa/ui/storage/baseinventory.h"
#include "asa/ui/info/dedicatedstorageinfo.h"

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
