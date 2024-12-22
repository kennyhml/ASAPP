#pragma once
#include <string>
#include "../interfaces/actionwheel.h"

namespace asa::structures
{
    class BaseStructure
    {
    public:
        explicit BaseStructure(std::string t_name) : name_(std::move(t_name)) {}

        /**
         * @brief Gets the name of the structure. 
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

    private:
        std::string name_;
        interfaces::ActionWheel action_wheel_;
    };
}
