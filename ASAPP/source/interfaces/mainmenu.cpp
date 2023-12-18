#include "asapp/interfaces/mainmenu.h"
#include "../util/util.h"
#include "asapp/game/resources.h"

namespace asa::interfaces
{
    bool MainMenu::is_open() const
    {
        return window::match_template(join_last_session_button.area,
                                      resources::interfaces::join_last_session);
    }

    bool MainMenu::got_connection_timeout() const
    {
        return window::match_template(accept.area, resources::interfaces::accept);
    }

    void MainMenu::accept_popup()
    {
        if (!got_connection_timeout()) { return; }

        do { accept.press(); }
        while (!util::await([this]() { return !got_connection_timeout(); },
                            std::chrono::seconds(5)));
    }

    void MainMenu::start()
    {
        if (!is_open()) { return; }
        do { start_button.press(); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }

    void MainMenu::join_last_session()
    {
        if (!is_open()) { return; }
        do { join_last_session_button.press(); }
        while (!util::await([this]() { return !is_open(); }, std::chrono::seconds(5)));
    }
}
