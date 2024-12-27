#pragma once
#include <filesystem>

#include "settings.h"
#include "asa/core/exceptions.h"

namespace asa
{
    struct window_not_found final : asapp_error
    {
    public:
        window_not_found() : asapp_error("The game window could not be located!") {}
    };

    struct invalid_action_mapping final : asapp_error
    {
    public:
        explicit invalid_action_mapping(const action_mapping& t_action_mapping)
            : asapp_error(std::format("Key '{}' of action mapping '{}' is not supported!",
                                      t_action_mapping.key, t_action_mapping.name)) {}
    };

    struct invalid_key_input final : asapp_error
    {
    public:
        explicit invalid_key_input(const std::string& t_key)
            : asapp_error(std::format("Key '{}' is not supported!", t_key)) {}
    };

    struct tesseract_not_initialized final : asapp_error
    {
    public:
        explicit tesseract_not_initialized(const std::filesystem::path& t_tessdata_path)
            : asapp_error(std::format(
                "Could not initialize tesseract with tessdata path '{}'!",
                t_tessdata_path.string())) {}
    };
}
