#include "asa/ui/main_menu.h"
#include "../../include/asa/utility.h"

namespace asa
{
    bool main_menu::is_open() const
    {
        return window::match(embedded::interfaces::join_last_session,
                             join_last_session_button.area
        );
    }

    bool main_menu::got_connection_timeout() const
    {
        return window::match(embedded::interfaces::accept, accept.area);
    }

    void main_menu::accept_popup()
    {
        if (!got_connection_timeout()) { return; }

        do { accept.press(); } while (!utility::await([this] {
            return !got_connection_timeout();
        }, 5s));
    }

    void main_menu::start()
    {
        if (!is_open()) { return; }
        do { start_button.press(); } while (!utility::await(
            [this] { return !is_open(); }, 5s));
    }

    void main_menu::join_last_session()
    {
        if (!is_open()) { return; }
        do {
            join_last_session_button.press();
        } while (!utility::await([this]() { return !is_open(); }, 5s));
    }
}
