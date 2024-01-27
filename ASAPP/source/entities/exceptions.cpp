#include "asapp/entities/exceptions.h"
#include "asapp/entities/localplayer.h"

namespace asa::entities
{
    EntityError::EntityError(const BaseEntity* t_entity)
            : info("Unspecified EntityError"), entity(t_entity) {}

    EntityError::EntityError(const BaseEntity* t_entity, std::string t_info)
            : info("EntityError: " + std::move(t_info)), entity(t_entity) {}

    const char* EntityError::what() const noexcept { return info.c_str(); }

    EntityNotAccessed::EntityNotAccessed(const BaseEntity* entity)
            : EntityError(entity, "Failed to access " + entity->get_name()) {}


    EntityNotMounted::EntityNotMounted(const DinoEnt* mount)
            : EntityError(mount, "Failed to mount " + mount->get_name()) {}

    SuicideFailedError::SuicideFailedError()
            : EntityError(local_player.get(), "Failed to suicide.") {}

    FastTravelFailedError::FastTravelFailedError(const std::string& t_where)
            : EntityError(local_player.get(),
                          std::format("Failed to travel to '{}'", t_where)) {}

    FastTravelFailedError::FastTravelFailedError(const std::string& t_where,
                                                 const std::string& t_why) : EntityError(
            local_player.get(),
            std::format("Failed to travel to '{}' - {}", t_where, t_why)) {}


    TeleportFailedError::TeleportFailedError(const std::string& t_where,
                                             const std::string& t_why) : EntityError(
            local_player.get(),
            std::format("Failed to teleport to '{}' - {}", t_where, t_why)) {}


}

