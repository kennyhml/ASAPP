#include "asapp/interfaces/containerinfo.h"

namespace asa::interfaces
{
    float ContainerInfo::get_fill_level()
    {
        auto roi = slots_bar;
        roi.y -= 5;

        auto masked = window::get_mask(roi, window::Color(1, 156, 136), 10);
        int greenPixels = cv::countNonZero(masked);

        return static_cast<float>(greenPixels) / static_cast<float>(slots_bar.width);
    }

    int ContainerInfo::get_max_weight() { return std::numeric_limits<float>::infinity(); }

    int ContainerInfo::get_current_weight() { return 0; }
}
