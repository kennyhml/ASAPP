#pragma once
#include "container.h"

namespace asa::structures
{
    class LargeCropPlot final : public Container
    {
    public:
        LargeCropPlot(std::string name) : Container(name, 10){};
    };
}

