#pragma once
#include "baseentity.h"
#include "asapp/interfaces/dinoinventory.h"

namespace asa::entities
{
    class DinoEntity : public BaseEntity
    {
    public:
        explicit DinoEntity(const std::string& t_name) : BaseEntity(t_name,
            std::make_unique<interfaces::DinoInventory>()) {}

        [[nodiscard]] interfaces::DinoInventory* get_inventory() const override
        {
            return dynamic_cast<interfaces::DinoInventory*>(inventory_.get());
        }

        [[nodiscard]] bool is_mounted() const;

        /**
         * @brief Opens the inventory of the dino.
         *
         * TODO: Create an inventory subclass to allow the inventory itself to handle things.
         */
        void open_inventory() const;
    };
}
