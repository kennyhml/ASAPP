#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/ui/info/baseentityinfo.h"

namespace asa
{
    namespace
    {
        float get_fill_amount(const cv::Rect& bar)
        {
            static cv::Vec3b color{1, 156, 136};

            const cv::Mat img = screenshot(bar);
            const int filled = utility::count_matches(img, color, 20);
            return static_cast<float>(filled) / static_cast<float>(bar.width);
        }
    }

    base_entity_info::base_entity_info()
    {
        for (int i = 0; i < gear_slots_.max_size(); i++) {
            gear_slots_[i] = gear_slot(764 + (305 * (i > 2)), 178 + (93 * (i % 3)));
        }
    }

    float base_entity_info::get_health_level()
    {
        static const cv::Rect roi(764, 514, 336, 1);
        return get_fill_amount(roi);
    }

    float base_entity_info::get_food_level()
    {
        static const cv::Rect roi(764, 619, 336, 1);
        return get_fill_amount(roi);
    }

    float base_entity_info::get_water_level()
    {
        static const cv::Rect roi(764, 654, 336, 1);
        return get_fill_amount(roi);
    }

    void base_entity_info::unequip(const slot slot)
    {
        const auto& gear_slot = gear_slots_.at(slot);

        while (!gear_slots_.empty()) {
            auto point = utility::center_of(gear_slot.area);
            checked_sleep(5ms);
            post_press(MouseButton::LEFT, point);
            checked_sleep(10ms);
        }
    }

    void base_entity_info::unequip_all()
    {
        for (int i = 0; i < gear_slots_.size(); i++) {
            unequip(static_cast<slot>(i));
        }
    }
}
