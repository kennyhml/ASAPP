#pragma once
#include "interactable.h"
#include "asa/interfaces/inventories/baseinventory.h"
#include "asa/interfaces/info/containerinfo.h"

namespace asa
{
    class container : public interactable
    {
    public:
        container(std::string t_name, int t_max_slots,
                  std::unique_ptr<base_inventory> t_inv = nullptr,
                  std::unique_ptr<container_info> t_info = nullptr);

        /**
         * @brief Gets the inventory component of the container. 
         */
        [[nodiscard]] virtual base_inventory* get_inventory() const
        {
            return dynamic_cast<base_inventory*>(interface_.get());
        }

        /**
         * @brief Gets the info component of the container. 
         */
        [[nodiscard]] virtual container_info* get_info() const
        {
            return info_.get();
        }

        /**
         * @brief Gets the maximum number of slots this container can hold. 
         */
        [[nodiscard]] int get_max_slots() const { return max_slots_; }

        /**
         * @brief Returns the last amount of slots that was known for this Container.
         */
        [[nodiscard]] int get_last_known_slots() const { return last_known_slots_; }

        /**
         * @brief Determines the amounts of slots in the Container, updates the last known
         * amount and returns the result.
         */
        int get_current_slots()
        {
            last_known_slots_ = get_info()->get_fill_level() * max_slots_;
            return last_known_slots_;
        }

    protected:
        int max_slots_;
        int last_known_slots_{0};
        std::unique_ptr<container_info> info_;
    };
}
