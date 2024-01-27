#pragma once
#include <exception>
#include <format>
#include <typeinfo>
#include "basestructure.h"

namespace asa::structures
{
    class StructureError : public std::exception
    {
        const BaseStructure* structure;
        std::string info;

    public:
        StructureError(const BaseStructure* t_structure);
        StructureError(const BaseStructure* t_structure, std::string t_info);

        const char* what() const noexcept override;
        const BaseStructure* get_structure() { return structure; }
    };

    class StructureNotOpenedError : public StructureError
    {
    public:
        StructureNotOpenedError(const BaseStructure* structure);
    };

    /**
     * @Brief thrown when a structure was not found in access range.
     */
    class StructureNotFoundError : public StructureError
    {
    public:
        StructureNotFoundError(const BaseStructure* structure);
    };


    class StructureNotClosedError : public StructureError
    {
    public:
        StructureNotClosedError(const BaseStructure* structure);
    };
}
