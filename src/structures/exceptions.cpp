#include "asa/structures/exceptions.h"
#include <format>

namespace asa
{
    structure_error::structure_error(const base_structure* t_structure,
                                     std::string t_info)
        : asapp_error(std::move(t_info)), structure(t_structure) {}

    structure_error::structure_error(const base_structure* t_structure)
        : structure_error(t_structure, "Unknown structure_error!") {};

    const char* structure_error::what() const noexcept { return this->info.c_str(); }
}
