#pragma once
#include "container.h"
#include "../interfaces/baseinventory.h"

namespace asa::structures
{
    class DedicatedStorage final : public Container
    {
    public:
        DedicatedStorage() : Container("Dedicated Storage", 1800) {}

        /**
         * @brief Checks whether depositing into the dedicated storage is possible.
         * 
         * @return True if we can deposit items into the dedi, false otherwise.
         */
        [[nodiscard]] bool can_deposit() const;
    };
}
