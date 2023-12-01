#include "asapp/structures/exceptions.h"
#include "../util/util.h"
#include <format>

asa::structures::exceptions::StructureError::StructureError(
	BaseStructure* structure, std::string message)
	: message(std::format(
		  "Structure Error at {}: {}", util::GetName(*structure), message)){};

asa::structures::exceptions::StructureError::StructureError(
	BaseStructure* structure)
	: message(
		  std::format("Structure Error at {}", util::GetName(*structure))){};

const char* asa::structures::exceptions::StructureError::what()
{
	return this->message.c_str();
}