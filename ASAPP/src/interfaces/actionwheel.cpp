#include <iostream>
#include "asa/interfaces/actionwheel.h"
#include "asa/core/state.h"
#include "asa/game/resources.h"

namespace asa::interfaces
{
    bool ActionWheel::is_open() const
    {
        window::Color white(126, 216, 236);

        auto masked = window::get_mask(name_area, white, 30);
        return cv::countNonZero(masked) > 100;
    }

    void ActionWheel::select_lay_on() const
    {
        auto matchLoc = window::locate_template(this->area, resources::interfaces::lay_on,
                                                0.9);

        if (!matchLoc.has_value()) {
            std::cerr << "[!] Lay on option not found! " << std::endl;
            return;
        }

        auto randPoint = matchLoc.value().get_random_location(5);
        window::set_mouse_pos(this->area.x + randPoint.x, this->area.y + randPoint.y);
        core::sleep_for(std::chrono::milliseconds(400));
        controls::mouse_press(controls::LEFT);
    }
}
