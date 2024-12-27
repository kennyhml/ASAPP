#include "asa/ui/menu.h"
#include "asa/ui/exceptions.h"
#include "asa/game/window.h"
#include "asa/utility.h"
#include "asa/core/state.h"

namespace asa
{
    bool menu::is_open() const
    {
        return match(embedded::text::resume, cv::Rect(894, 416, 143, 35));
    }

    void menu::open()
    {
        const auto start = std::chrono::system_clock::now();

        while (!is_open()) {
            post_press("esc");

            if (utility::await([this] { return is_open(); }, 3s)) {
                break;
            }

            // Increased timeout to 60 seconds
            if (utility::timedout(start, 60s)) {
                throw failed_to_open(this);
            }
        }
        checked_sleep(1s);
    }

    void menu::close()
    {
        const auto start = std::chrono::system_clock::now();

        while (is_open()) {
            post_press("esc");

            if (utility::await([this] { return !is_open(); }, 3s)) {
                break;
            }

            // Increased timeout to 60 seconds
            if (utility::timedout(start, 60s)) {
                throw failed_to_close(this);
            }
        }
        checked_sleep(1s);
    }
}
