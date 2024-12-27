#include "asa/ui/components/button.h"

#include "asa/utility.h"

namespace asa
{
    button::button(const int t_x, const int t_y, const int t_width, const int t_height,
                   const int t_padding)
        : interface_component(t_x, t_y, t_width, t_height), padding(t_padding) {};

    button::button(cv::Rect t_area, int t_padding)
        : interface_component(t_area), padding(t_padding) {};

    void button::press()
    {
        post_press(MouseButton::LEFT, utility::center_of(area));

        last_pressed = std::chrono::system_clock::now();
        last_button_press = last_pressed;
    };
}
