#include "asapp/entities/exceptions.h"

using namespace asa::entities::exceptions;

EntityError::EntityError(const BaseEntity* entity)
	: entity(entity), info("Unspecified EntityError"){};

EntityError::EntityError(const BaseEntity* entity, std::string info)
	: entity(entity), info("EntityError: " + info){};

const char* EntityError::what() const noexcept { return this->info.c_str(); }

EntityNotAccessed::EntityNotAccessed(const BaseEntity* entity)
	: EntityError(entity, "Failed to access " + entity->GetName()){};

EntityNotClosed::EntityNotClosed(const BaseEntity* entity)
	: EntityError(entity, "Failed to close " + entity->GetName()){};

EntityNotMounted::EntityNotMounted(const DinoEnt* mount)
	: EntityError(mount, "Failed to mount " + mount->GetName()){};
