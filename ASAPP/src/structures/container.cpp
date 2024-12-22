#include "asa/structures/container.h"


namespace asa::structures
{
    Container::Container(std::string t_name, const int t_max_slots,
                         std::unique_ptr<interfaces::BaseInventory> t_inv,
                         std::unique_ptr<interfaces::ContainerInfo> t_info)
        : Interactable(std::move(t_name), &settings::action_mappings::access_inventory,
                       t_inv
                           ? std::move(t_inv)
                           : std::make_unique<interfaces::BaseInventory>(true)),
          max_slots_(t_max_slots),
          info_(t_info
                    ? std::move(t_info)
                    : std::make_unique<interfaces::ContainerInfo>()) {}
}
