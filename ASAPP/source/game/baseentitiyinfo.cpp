#include <iostream>

#include "asapp/interfaces/baseentityinfo.h"


namespace asa::interfaces
{
    namespace
    {
        float get_fill_amount(const window::Rect& bar)
        {
            static constexpr window::Color color{1, 156, 136};

            const int filled = cv::countNonZero(window::get_mask(bar, color, 20));
            return static_cast<float>(filled) / static_cast<float>(bar.width);
        }
    }

    float BaseEntityInfo::get_health_level()
    {
        static const window::Rect roi(764, 514, 336, 1);
        return get_fill_amount(roi);
    }

    float BaseEntityInfo::get_food_level()
    {
        static const window::Rect roi(764, 619, 336, 1);
        return get_fill_amount(roi);
    }

    float BaseEntityInfo::get_water_level()
    {
        static const window::Rect roi(764, 654, 336, 1);
        return get_fill_amount(roi);
    }
}
