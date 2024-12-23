#include "asa/interfaces/mode_select.h"
#include "../../include/asa/utility.h"
#include "asa/game/resources.h"


namespace asa::interfaces
{
    bool mode_select::is_open() const
    {
        return window::match_template(back_button.area, resources::interfaces::back);
    }

    void mode_select::join_game()
    {
        if(!util::await([this]{return is_open();}, std::chrono::seconds(5))) {
            return;
        }
        do { window::post_mouse_press_at({614,594}, controls::LEFT); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }
}
