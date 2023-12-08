#pragma once
#include "baseentity.h"
#include "dinoent.h"
#include <stdexcept>

namespace asa::entities::exceptions
{
	class EntityError : public std::exception
	{
	private:
		std::string info;

	public:
		const BaseEntity* entity;

		EntityError(const BaseEntity*);
		EntityError(const BaseEntity*, std::string info);

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