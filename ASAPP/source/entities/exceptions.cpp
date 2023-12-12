#include "asapp/entities/exceptions.h"

namespace asa::entities
{
	EntityError::EntityError(const BaseEntity* entity)
		: entity(entity), info("Unspecified EntityError"){};

	EntityError::EntityError(const BaseEntity* entity, std::string info)
		: entity(entity), info("EntityError: " + info){};

	const char* EntityError::what() const noexcept { return info.c_str(); }

	EntityNotAccessed::EntityNotAccessed(const BaseEntity* entity)
		: EntityError(entity, "Failed to access " + entity->get_name()){};

	EntityNotClosed::EntityNotClosed(const BaseEntity* entity)
		: EntityError(entity, "Failed to close " + entity->get_name()){};

	EntityNotMounted::EntityNotMounted(const DinoEnt* mount)
		: EntityError(mount, "Failed to mount " + mount->get_name()){};
}
