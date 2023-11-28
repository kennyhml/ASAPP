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
		StructureError(BaseStructure* structure, std::string message);
		StructureError(BaseStructure* structure);

		const char* what();
	};

	class StructureNotOpenedError : public StructureError
	{
		StructureNotOpenedError(BaseStructure* structure)
			: StructureError(structure, "Accessing structure failed."){};
	};

	class StructureNotClosedError : public StructureError
	{
		StructureNotClosedError(BaseStructure* structure)
			: StructureError(structure, "Exitting structure failed."){};
	};

}