#pragma once
#include <string>
#include "asa/interfaces/actionwheel.h"

namespace asa
{
    class base_structure
    {
    public:
        explicit base_structure(std::string t_name) : name_(std::move(t_name)) {}

        /**
         * @brief Gets the name of the structure. 
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

    private:
        std::string name_;
        interfaces::ActionWheel action_wheel_;
    };
}
