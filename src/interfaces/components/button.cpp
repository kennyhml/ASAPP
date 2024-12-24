#include "asa/interfaces/components/button.h"

#include "asa/utility.h"
#include "asa/game/controls.h"

namespace asa
{
    button::button(const int t_x, const int t_y, const int t_width, const int t_height,
                   const int t_padding)
        : interface_component(t_x, t_y, t_width, t_height), padding(t_padding) {};

    button::button(cv::Rect t_area, int t_padding) : interface_component(t_area),
                                                     padding(t_padding) {};

    void button::press()
    {
        window::post_mouse_press_at(utility::center_of(area), controls::LEFT);

        last_pressed = std::chrono::system_clock::now();
        last_button_press = last_pressed;
    };
}
