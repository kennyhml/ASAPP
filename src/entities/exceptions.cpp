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

    entity_mount_failed::entity_mount_failed(const base_entity* t_entity)
        : entity_error(t_entity,
                       std::format("Failed to mount '{}'!", t_entity->get_name())) {}

    suicide_failed::suicide_failed()
        : entity_error(nullptr, "Failed to suicide!") {}

    travel_failed::travel_failed(const std::string& t_where)
        : entity_error(nullptr, std::format("Failed to travel to '{}'!", t_where)) {}

    travel_failed::travel_failed(const std::string& t_where,
                                 const std::string& t_why)
        : entity_error(nullptr, std::format("Failed to travel to '{}': {}!", t_where, t_why)) {}
}
