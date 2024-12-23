#include "asa/interfaces/main_menu.h"
#include "../../include/asa/utility.h"
#include "asa/game/resources.h"

namespace asa::interfaces
{
    bool main_menu::is_open() const
    {
        return window::match_template(join_last_session_button.area,
                                      resources::interfaces::join_last_session);
    }

    bool main_menu::got_connection_timeout() const
    {
        return window::match_template(accept.area, resources::interfaces::accept);
    }

    void main_menu::accept_popup()
    {
        if (!got_connection_timeout()) { return; }

        do { accept.press(); }
        while (!util::await([this]() { return !got_connection_timeout(); },
                            std::chrono::seconds(5)));
    }

    void main_menu::start()
    {
        if (!is_open()) { return; }
        do { start_button.press(); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }

    void main_menu::join_last_session()
    {
        if (!is_open()) { return; }
        do { join_last_session_button.press(); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }
}
