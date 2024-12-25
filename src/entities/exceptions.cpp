#include "asa/entities/exceptions.h"
#include "asa/entities/localplayer.h"

namespace asa
{
    entity_error::entity_error(const base_entity* t_entity)
        : asapp_error("Unspecified entity_error"), entity_(t_entity) {}

    entity_error::entity_error(const base_entity* t_entity, std::string t_info)
        : asapp_error("entity_error: " + std::move(t_info)), entity_(t_entity) {}

    entity_access_failed::entity_access_failed(const base_entity* t_entity)
        : entity_error(t_entity,
                       std::format("Failed to access '{}'!", t_entity->get_name())) {}

    structure_access_failed::structure_access_failed(const base_structure& t_structure)
        : entity_error(
            nullptr, std::format("Failed to access '{}'!", t_structure.get_name())) {}

    target_not_in_range::target_not_in_range(const std::string& t_target)
        : entity_error(
            nullptr, std::format("Interaction target '{}' not in range!", t_target)) {}

    deposit_failed::deposit_failed(const std::string& t_what_item)
        : entity_error(nullptr, std::format("Failed to deposit {}!", t_what_item)) {}

    entity_mount_failed::entity_mount_failed(const base_entity* t_entity)
        : entity_error(t_entity,
                       std::format("Failed to mount '{}'!", t_entity->get_name())) {}

    suicide_failed::suicide_failed()
        : entity_error(nullptr, "Failed to suicide!") {}

    travel_failed::travel_failed(const std::string& t_where)
        : entity_error(nullptr, std::format("Failed to travel to '{}'!", t_where)) {}

    travel_failed::travel_failed(const std::string& t_where,
                                 const std::string& t_why)
        : entity_error(
            nullptr, std::format("Failed to travel to '{}': {}!", t_where, t_why)) {}
}
