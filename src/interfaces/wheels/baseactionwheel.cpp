#include "asa/ui/wheels/baseactionwheel.h"
#include "asa/utility.h"
#include "asa/core/state.h"

#include <iostream>

namespace asa
{
    bool base_action_wheel::is_open() const
    {
        static cv::Vec3b white(126, 216, 236);
        return utility::count_matches(name_area, white, 30) > 100;
    }

    void base_action_wheel::select_lay_on() const
    {
        const auto loc = window::locate(embedded::interfaces::lay_on, area, 0.9);
        if (!loc.has_value()) {
            std::cerr << "[!] Lay on option not found! " << std::endl;
            return;
        }

        const cv::Point point = utility::center_of(*loc);
        window::set_mouse_pos({area.x + point.x, area.y + point.y});
        checked_sleep(std::chrono::milliseconds(400));
        controls::mouse_press(controls::LEFT);
    }
}
