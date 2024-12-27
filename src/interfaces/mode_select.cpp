#include "asa/ui/mode_select.h"
#include "../../include/asa/utility.h"

namespace asa
{
    bool mode_select::is_open() const
    {
        return match(embedded::interfaces::back, back_button.area);
    }

    void mode_select::join_game()
    {
        if (!utility::await([this] { return is_open(); }, 5s)) {
            return;
        }
        do {
            post_press(MouseButton::LEFT, cv::Point{614, 594});
        } while (!utility::await([this]() { return !is_open(); }, 5s));
    }
}
