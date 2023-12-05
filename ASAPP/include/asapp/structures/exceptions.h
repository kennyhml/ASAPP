#pragma once
#include "basestructure.h"
#include <exception>
#include <format>
#include <typeinfo>

namespace asa::structures::exceptions
{
	class StructureError : public std::exception
	{
		std::string message;

	public:
		StructureError(const BaseStructure* structure, std::string message);
		StructureError(const BaseStructure* structure);

		const char* what() const noexcept override;
	};

	class StructureNotOpenedError : public StructureError
	{
		StructureNotOpenedError(const BaseStructure* structure)
			: StructureError(structure, "Accessing structure failed."){};
	};

	class StructureNotClosedError : public StructureError
	{
		StructureNotClosedError(const BaseStructure* structure)
			: StructureError(structure, "Exitting structure failed."){};
	};

}