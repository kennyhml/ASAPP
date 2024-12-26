#include "asa/utility.h"
#include "asa/ui/info/containerinfo.h"

namespace asa
{
    namespace
    {
        float get_filled(const cv::Rect& bar)
        {
            static cv::Vec3b fill_color{1, 156, 136};
            const int green = utility::count_matches(bar, fill_color, 10);

            return static_cast<float>(green) / static_cast<float>(bar.width);
        }
    }

    float container_info::get_fill_level()
    {
        auto roi = slots_bar_;
        roi.y -= 5;
        return get_filled(roi);
    }

    float container_info::get_health_level()
    {
        auto roi = health_bar_;
        roi.y -= 5;

        return get_filled(roi);
    }

    int container_info::get_max_weight() { return std::numeric_limits<float>::infinity(); }

    int container_info::get_current_weight() { return 0; }
}
