#include "asapp/structures/exceptions.h"
#include <format>
#include "asapp/util/util.h"


namespace asa::structures
{
    StructureError::StructureError(const BaseStructure* structure, std::string info) :
            structure(structure), info(std::format("StructureError: {}", info)) {};

    StructureError::StructureError(const BaseStructure* structure) : StructureError(
            structure, "Unspecified error") {};

    const char* StructureError::what() const noexcept { return this->info.c_str(); }


    StructureNotFoundError::StructureNotFoundError(
            const asa::structures::BaseStructure* structure)
            : StructureError(structure,
                             std::format("'{}' is not accessible.",
                                         structure->get_name())) {}

    StructureNotOpenedError::StructureNotOpenedError(const BaseStructure* structure) :
            StructureError(structure,
                           std::format("Failed to access '{}'",
                                       structure->get_name())) {};

    StructureNotClosedError::StructureNotClosedError(const BaseStructure* structure) :
            StructureError(structure,
                           std::format("Failed to exit '{}'", structure->get_name())) {};
}
