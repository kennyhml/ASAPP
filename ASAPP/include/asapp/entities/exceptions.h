#pragma once
#include "baseentity.h"
#include "dinoent.h"
#include <stdexcept>

namespace asa::entities::exceptions
{
	class EntityError : public std::exception
	{
	private:
		const BaseEntity* entity;
		std::string info;

	public:
		EntityError(const BaseEntity*);
		EntityError(const BaseEntity*, std::string info);

		const char* what();
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