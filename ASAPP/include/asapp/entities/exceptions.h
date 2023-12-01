#pragma once
#include "baseentity.h"
#include <stdexcept>

namespace asa::entities::exceptions
{
	class EntityError : public std::exception
	{
	private:
		BaseEntity* entity;
		std::string message;

	public:
		EntityError(BaseEntity*);
		EntityError(BaseEntity*, std::string message);

		const char* what();
	};

	class EntityNotAccessed : public EntityError
	{
	public:
		EntityNotAccessed(BaseEntity*);

		const char* what();
	};

	class EntityNotClosed : public EntityError
	{
	public:
		EntityNotClosed(BaseEntity*);

		const char* what();
	};

	class EntityNotMounted : public EntityError
	{
	public:
		EntityNotMounted(BaseEntity*);

		const char* what();
	};
}