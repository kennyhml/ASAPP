#pragma once
#include <string>
#include "../interfaces/actionwheel.h"

namespace asa::structures
{
    class BaseStructure
    {
    public:
        BaseStructure(std::string t_name) : name(t_name) {};

        const std::string name;
        interfaces::ActionWheel action_wheel;
    };
}
