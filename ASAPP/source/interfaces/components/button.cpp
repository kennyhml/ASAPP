#include "asapp/interfaces/components/button.h"
#include "asapp/game/controls.h"

namespace asa::interfaces::components
{
    Button::Button(int t_x, int t_y, int t_width, int t_height, int t_padding) :
        IInterfaceComponent(t_x, t_y, t_width, t_height), padding(t_padding) {};

    Button::Button(window::Rect t_area, int t_padding) : IInterfaceComponent(t_area),
                                                         padding(t_padding) {};

    void Button::press()
    {
        auto loc = area.get_random_location(padding);

        post_mouse_press_at(loc, controls::MouseButton::LEFT);

        last_pressed = std::chrono::system_clock::now();
        last_button_press = last_pressed;
    };
}
