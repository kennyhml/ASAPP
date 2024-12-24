#include "asa/interfaces/mode_select.h"
#include "../../include/asa/utility.h"

namespace asa
{
    bool mode_select::is_open() const
    {
        return window::match(embedded::interfaces::back, back_button.area);
    }

    void mode_select::join_game()
    {
        if (!utility::await([this] { return is_open(); }, 5s)) {
            return;
        }
        do {
            window::post_mouse_press_at({614, 594}, controls::LEFT);
        } while (!utility::await([this]() { return !is_open(); }, 5s));
    }
}
