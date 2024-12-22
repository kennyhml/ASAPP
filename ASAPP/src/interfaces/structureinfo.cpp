#include "asa/interfaces/containerinfo.h"

namespace asa::interfaces
{
    namespace
    {
        float get_filled(const window::Rect& bar)
        {
            static constexpr window::Color fill_color{1, 156, 136};
            const auto mask = window::get_mask(bar, fill_color, 10);
            const int green = cv::countNonZero(mask);

            return static_cast<float>(green) / static_cast<float>(bar.width);
        }
    }

    float ContainerInfo::get_fill_level()
    {
        auto roi = slots_bar_;
        roi.y -= 5;
        return get_filled(roi);
    }

    float ContainerInfo::get_health_level()
    {
        auto roi = health_bar_;
        roi.y -= 5;

        return get_filled(roi);
    }

    int ContainerInfo::get_max_weight() { return std::numeric_limits<float>::infinity(); }

    int ContainerInfo::get_current_weight() { return 0; }
}
