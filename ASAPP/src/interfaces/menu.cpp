#include "asa/interfaces/menu.h"

#include "asa/core/state.h"
#include "asa/game/resources.h"
#include "asa/game/window.h"
#include "asa/interfaces/exceptions.h"
#include "../../include/asa/utility.h"

namespace asa::interfaces
{
    bool menu::is_open() const
    {
        return window::match_template(window::Rect(894, 416, 143, 35),
                                      resources::text::resume);
    }

    void menu::open()
    {
        const auto start = std::chrono::system_clock::now();

        while (!is_open()) {
            controls::key_press("esc");

            if (util::await([this] { return is_open(); }, 3s)) {
                break;
            }

            // Increased timeout to 60 seconds
            if (util::timedout(start, 60s)) {
                throw failed_to_open(this);
            }
        }
        core::sleep_for(1s);
    }

    void menu::close()
    {
        const auto start = std::chrono::system_clock::now();

        while (is_open()) {
            controls::key_press("esc");

            if (util::await([this] { return !is_open(); }, 3s)) {
                break;
            }

            // Increased timeout to 60 seconds
            if (util::timedout(start, 60s)) {
                throw failed_to_close(this);
            }
        }
        core::sleep_for(1s);
    }
}
