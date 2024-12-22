#pragma once
#include "container.h"

namespace asa::structures
{
    class MediumCropPlot final : public Container
    {
    public:
        MediumCropPlot(std::string name) : Container(name, 10) {};
    };
}
