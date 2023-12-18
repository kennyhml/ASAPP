#include "asapp/entities/exceptions.h"

namespace asa::entities
{
    EntityError::EntityError(const BaseEntity* t_entity) :
        info("Unspecified EntityError"), entity(entity) {};

    EntityError::EntityError(const BaseEntity* t_entity, std::string t_info) :
        info("EntityError: " + info), entity(entity) {};

    const char* EntityError::what() const noexcept { return info.c_str(); }

    EntityNotAccessed::EntityNotAccessed(const BaseEntity* entity) : EntityError(
        entity, "Failed to access " + entity->get_name()) {};

    EntityNotClosed::EntityNotClosed(const BaseEntity* entity) : EntityError(
        entity, "Failed to close " + entity->get_name()) {};

    EntityNotMounted::EntityNotMounted(const DinoEnt* mount) : EntityError(
        mount, "Failed to mount " + mount->get_name()) {};
}
