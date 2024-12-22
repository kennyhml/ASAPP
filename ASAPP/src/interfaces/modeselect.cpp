#include "asa/interfaces/modeselect.h"
#include "asa/util/util.h"
#include "asa/game/resources.h"


namespace asa::interfaces
{
    bool ModeSelect::is_open() const
    {
        return window::match_template(back_button.area, resources::interfaces::back);
    }

    void ModeSelect::join_game()
    {
        if(!util::await([this]{return is_open();}, std::chrono::seconds(5))) {
            return;
        }
        do { window::post_mouse_press_at({614,594}, controls::LEFT); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }
}
