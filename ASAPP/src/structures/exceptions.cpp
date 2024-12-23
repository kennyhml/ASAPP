#include "asa/structures/exceptions.h"
#include <format>
#include "../../include/asa/utility.h"


namespace asa::structures
{
    structure_error::structure_error(const BaseStructure* structure, std::string info) :
            structure(structure), info(std::format("StructureError: {}", info)) {};

    structure_error::structure_error(const BaseStructure* structure) : structure_error(
            structure, "Unspecified error") {};

    const char* structure_error::what() const noexcept { return this->info.c_str(); }


    StructureNotFoundError::StructureNotFoundError(
            const asa::structures::BaseStructure* structure)
            : structure_error(structure,
                             std::format("'{}' is not accessible.",
                                         structure->get_name())) {}

    StructureNotOpenedError::StructureNotOpenedError(const BaseStructure* structure) :
            structure_error(structure,
                           std::format("Failed to access '{}'",
                                       structure->get_name())) {};

    StructureNotClosedError::StructureNotClosedError(const BaseStructure* structure) :
            structure_error(structure,
                           std::format("Failed to exit '{}'", structure->get_name())) {};
}
