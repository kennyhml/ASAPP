#include "asa/structures/container.h"


namespace asa
{
    container::container(std::string t_name, const int t_max_slots,
                         std::unique_ptr<base_inventory> t_inv,
                         std::unique_ptr<container_info> t_info)
        : interactable(std::move(t_name), &get_action_mapping("AccessInventory"),
                       t_inv ? std::move(t_inv) : std::make_unique<base_inventory>(true)),
          max_slots_(t_max_slots),
          info_(t_info ? std::move(t_info) : std::make_unique<container_info>()) {}
}
