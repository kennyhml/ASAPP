#pragma once
#include "asa/core/exceptions.h"
#include <string>
#include <format>

namespace asa
{
    struct item_not_found final : asapp_error
    {
    public:
        explicit item_not_found(const std::string& t_item)
            : asapp_error(std::format("Unknown item requested: '{}'!", t_item)) {}
    };

    struct item_icon_not_found final : asapp_error
    {
        explicit item_icon_not_found(const std::string& t_item)
            : asapp_error(std::format("Item '{}' is missing icon resource!", t_item)) {}
    };

    struct missing_item_field final : asapp_error
    {
        missing_item_field(const std::string& t_item, const std::string& t_field)
            : asapp_error(std::format("Item '{}' is missing '{}'!", t_item, t_field)) {}
    };
}
