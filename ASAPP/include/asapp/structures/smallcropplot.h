#pragma once
#include "container.h"

namespace asa::structures
{
    class SmallCropPlot final : public Container
    {
    public:
        SmallCropPlot(std::string name) : Container(name, 10) {};
    };
}
