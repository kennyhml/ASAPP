#pragma once
#include "baseentity.h"
#include "dinoent.h"
#include <stdexcept>

namespace asa::entities
{
	class EntityError : public std::exception
	{
	private:
		std::string info;
		const BaseEntity* entity;

	public:
		EntityError(const BaseEntity* t_entity);
		EntityError(const BaseEntity* t_entity, std::string t_info);

		const BaseEntity* get_entity() const { return entity; };
		const char* what() const noexcept override;
	};

	class EntityNotAccessed : public EntityError
	{
	public:
		EntityNotAccessed(const BaseEntity*);
	};

	class EntityNotClosed : public EntityError
	{
	public:
		EntityNotClosed(const BaseEntity*);
	};

	class EntityNotMounted : public EntityError
	{
	public:
		EntityNotMounted(const DinoEnt*);
	};
}