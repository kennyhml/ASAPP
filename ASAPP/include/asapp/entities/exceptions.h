#pragma once
#include "baseentity.h"
#include <stdexcept>

namespace asa::entities::exceptions
{
	class EntityError : public std::exception
	{
	private:
		const BaseEntity* entity;
		std::string message;

	public:
		EntityError(const BaseEntity*);
		EntityError(const BaseEntity*, std::string message);

		const char* what();
	};

	class EntityNotAccessed : public EntityError
	{
	public:
		EntityNotAccessed(const BaseEntity*);

		const char* what();
	};

	class EntityNotClosed : public EntityError
	{
	public:
		EntityNotClosed(const BaseEntity*);

		const char* what();
	};

	class EntityNotMounted : public EntityError
	{
	public:
		EntityNotMounted(const BaseEntity*);

		const char* what();
	};
}