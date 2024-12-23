#pragma once
#include "baseentity.h"
#include "asa/interfaces/inventories/dinoinventory.h"

namespace asa
{
    /**
     * @brief Basic dino entity implementation.
     *
     * Provides access to the more concrete dino inventory.
     */
    class dino_entity : public base_entity
    {
    public:
        explicit dino_entity(const std::string& t_name)
            : base_entity(t_name, std::make_unique<dino_inventory>()) {}

        /**
         * @brief Gets the inventory component of the dinosaur.
         *
         * Provides more concrete control over dino stats, inventory and saddles.
         */
        [[nodiscard]] dino_inventory* get_inventory() const override
        {
            return dynamic_cast<dino_inventory*>(inventory_.get());
        }
    };
}
