#pragma once
#include "baseentity.h"

namespace asa::entities
{
    class DinoEnt : public BaseEntity
    {
    public:
        using BaseEntity::BaseEntity;

        [[nodiscard]] bool is_mounted() const;

        /**
         * @brief Opens the inventory of the dino.
         *
         * TODO: Create an inventory subclass to allow the inventory itself to handle things.
         */
        void open_inventory() const;
    };
}
