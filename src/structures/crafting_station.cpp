#include "asa/structures/crafting_station.h"


namespace asa
{
    crafting_station::crafting_station(std::string t_name, const int t_max_slots,
                                       std::unique_ptr<crafting_inventory> t_inv,
                                       std::unique_ptr<container_info> t_info)
        : container(std::move(t_name), t_max_slots,
                    t_inv ? std::move(t_inv) : std::make_unique<crafting_inventory>(),
                    std::move(t_info)) {}
}
