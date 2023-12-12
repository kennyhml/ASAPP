#include "asapp/structures/exceptions.h"
#include "../util/util.h"
#include <format>


namespace asa::structures
{
	StructureError::StructureError(
		const BaseStructure* structure, std::string info)
		: info(std::format("StructureError: {}", info)), structure(structure){};

	StructureError::StructureError(const BaseStructure* structure)
		: StructureError(structure, "Unspecified error"){};

	const char* StructureError::what() const noexcept
	{
		return this->info.c_str();
	}

	StructureNotOpenedError::StructureNotOpenedError(
		const BaseStructure* structure)
		: StructureError(structure,
			  std::format("Failed to access '{}'", structure->name)){};

	StructureNotClosedError::StructureNotClosedError(
		const BaseStructure* structure)
		: StructureError(
			  structure, std::format("Failed to exit '{}'", structure->name)){};
}