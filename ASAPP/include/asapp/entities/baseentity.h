#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"

namespace asa::entities
{
    class BaseEntity
    {
    public:
        explicit BaseEntity(std::string t_name);
        virtual ~BaseEntity() = default;

        /**
         * @brief Gets the name of the entity.
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

        /**
         * @brief Gets the inventory component of the entity.
         */
        [[nodiscard]] virtual interfaces::BaseInventory* get_inventory() const
        {
            return inventory_.get();
        }

    protected:
        std::string name_;
        std::unique_ptr<interfaces::BaseInventory> inventory_;
        interfaces::ActionWheel action_wheel_;
    };
}
