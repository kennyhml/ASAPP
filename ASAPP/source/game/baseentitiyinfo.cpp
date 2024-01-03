#include <iostream>

#include "asapp/interfaces/baseentityinfo.h"


namespace asa::interfaces
{
    float BaseEntityInfo::get_health_percentage()
    {
        static const window::Rect roi(764, 514, 336, 1);
        static constexpr window::Color color{1, 156, 136};

        const int filled = cv::countNonZero(window::get_mask(roi, color, 20));
        return static_cast<float>(filled) / static_cast<float>(roi.width);
    }
}
