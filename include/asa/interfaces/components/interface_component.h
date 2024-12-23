#pragma once
#include "asa/game/window.h"

namespace asa
{
    struct interface_component
    {
        interface_component() : interface_component(0, 0, 0, 0) {};
        explicit interface_component(window::Rect t_area) : area(t_area) {};

        interface_component(int t_x, int t_y, int t_width, int t_height) : area(
            window::Rect(t_x, t_y, t_width, t_height)) {};

        window::Rect area;
    };
}
