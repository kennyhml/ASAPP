#include "asa/structures/tributeable.h"

namespace asa::structures
{
    tributable::Tributable(std::string t_name, const int t_max_slots,
                           std::unique_ptr<interfaces::TributeInventory> t_inv,
                           std::unique_ptr<interfaces::container_info> t_info)
        : Container(std::move(t_name), t_max_slots,
                    t_inv
                        ? std::move(t_inv)
                        : std::make_unique<interfaces::TributeInventory>(),
                    std::move(t_info)) {}
}
