#pragma once
#include "interactablestructure.h"
#include "../interfaces/baseinventory.h"
#include "../interfaces/containerinfo.h"

namespace asa::structures
{
    class Container : public InteractableStructure
    {
    public:
        Container(std::string t_name, int t_max_slots,
                  std::unique_ptr<interfaces::BaseInventory> t_inv = nullptr,
                  std::unique_ptr<interfaces::ContainerInfo> t_info = nullptr);

        /**
         * @brief Gets the inventory component of the container. 
         */
        [[nodiscard]] virtual interfaces::BaseInventory* get_inventory() const
        {
            return dynamic_cast<interfaces::BaseInventory*>(interface_.get());
        }

        /**
         * @brief Gets the info component of the container. 
         */
        [[nodiscard]] virtual interfaces::ContainerInfo* get_info() const
        {
            return info_.get();
        }

        /**
         * @brief Gets the maximum number of slots this container can hold. 
         */
        [[nodiscard]] int get_max_slots() const { return max_slots_; }

        [[nodiscard]] int get_current_slots() const
        {
            return get_info()->get_fill_level() * max_slots_;
        }

    protected:
        int max_slots_;
        std::unique_ptr<interfaces::ContainerInfo> info_;
    };
}
