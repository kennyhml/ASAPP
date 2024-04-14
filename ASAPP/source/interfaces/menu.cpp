#include "asapp/interfaces/menu.h"

#include "asapp/core/state.h"
#include "asapp/game/resources.h"
#include "asapp/game/window.h"
#include "asapp/interfaces/exceptions.h"
#include "asapp/util/util.h"

namespace asa::interfaces
{
    bool Menu::is_open() const
    {
        return window::match_template(window::Rect(916, 439, 90, 35),
                                      resources::text::resume);
    }

    void Menu::open()
    {
        const auto start = std::chrono::system_clock::now();

        while (!is_open()) {
            controls::key_press("esc");

            if (util::await([this] { return is_open(); }, 3s)) {
                break;
            }
            if (util::timedout(start, 30s)) {
                throw InterfaceNotOpenedError(this);
            }
        }
        core::sleep_for(1s);
    }

    void Menu::close()
    {
        const auto start = std::chrono::system_clock::now();

        while (is_open()) {
            controls::key_press("esc");

            if (util::await([this] { return !is_open(); }, 3s)) {
                break;
            }
            if (util::timedout(start, 30s)) {
                throw InterfaceNotClosedError(this);
            }
        }
        core::sleep_for(1s);
    }
}
