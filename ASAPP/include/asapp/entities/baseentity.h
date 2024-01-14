#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"

namespace asa::entities
{
    class BaseEntity
    {
    public:
        explicit BaseEntity(std::string t_name,
                            std::unique_ptr<interfaces::BaseInventory> t_inventory =
                                nullptr) : name_(std::move(t_name)),
                                           inventory_(
                                               t_inventory
                                                   ? std::move(t_inventory)
                                                   : std::make_unique<
                                                       interfaces::BaseInventory>(true)) {
        }

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

        /**
         * @brief Uses the primary attack of the entity (left click) 
         */
        void primary_attack();

        /**
         * @brief Uses the secondary attack of the entity (right click) 
         */
        void secondary_attack();

    protected:
        std::string name_;
        std::unique_ptr<interfaces::BaseInventory> inventory_;
        interfaces::ActionWheel action_wheel_;
    };
}
