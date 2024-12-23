#pragma once
#include <format>
#include "basestructure.h"
#include "asa/core/exceptions.h"

namespace asa
{
    class structure_error : public asapp_error
    {
        const base_structure* structure;
        std::string info;

    public:
        explicit structure_error(const base_structure* t_structure);
        structure_error(const base_structure* t_structure, std::string t_info);

        const char* what() const noexcept override;

        const base_structure* get_structure() { return structure; }
    };
}
