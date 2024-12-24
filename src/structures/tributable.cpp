#include "asa/structures/tributeable.h"

namespace asa
{
    tributable::tributable(std::string t_name, const int t_max_slots,
                           std::unique_ptr<tribute_inventory> t_inv,
                           std::unique_ptr<container_info> t_info)
        : container(std::move(t_name), t_max_slots,
                    t_inv ? std::move(t_inv) : std::make_unique<tribute_inventory>(),
                    std::move(t_info)) {}
}
