#pragma once
#include "asa/interfaces/wheels/baseactionwheel.h"
#include <string>

namespace asa
{
    class base_structure
    {
    public:
        virtual ~base_structure() = default;

        explicit base_structure(std::string t_name)
            : name_(std::move(t_name)) {}

        /**
         * @brief Gets the name of the structure. 
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

    private:
        std::string name_;
        base_action_wheel action_wheel_;
    };
}
