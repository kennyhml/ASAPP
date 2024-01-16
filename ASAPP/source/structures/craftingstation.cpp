#include "asapp/structures/craftingstation.h"


namespace asa::structures
{
    CraftingStation::CraftingStation(std::string t_name, const int t_max_slots,
                                     std::unique_ptr<interfaces::CraftingInventory> t_inv,
                                     std::unique_ptr<interfaces::ContainerInfo> t_info) :
        Container(std::move(t_name), t_max_slots,
                  t_inv
                      ? std::move(t_inv)
                      : std::make_unique<interfaces::CraftingInventory>(),
                  std::move(t_info)) {}
}
