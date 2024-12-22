#pragma once
#include "baseentity.h"
#include "asa/interfaces/dinoinventory.h"

namespace asa::entities
{
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

        /**
         * @brief Checks whether the dino is currently mounted by the local player.
         *
         * @return True if the dino is mounted, false otherwise.
         */
        [[nodiscard]] bool is_mounted() const;

        /**
         * @brief Opens the inventory of the dino.
         *
         * TODO: Create an inventory subclass to allow the inventory itself to handle things.
         */
        void open_inventory() const;
    };
}
