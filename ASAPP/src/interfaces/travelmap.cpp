#include <iostream>
#include "asa/interfaces/travelmap.h"
#include "asa/util/util.h"
#include "asa/core/state.h"
#include "asa/game/resources.h"
#include "asa/interfaces/exceptions.h"

namespace asa::interfaces
{
    namespace
    {
        bool open()
        {
            return window::match_template(window::Rect(283, 129, 91, 46),
                                          resources::text::beds);
        }
    }

    bool TravelMap::is_open() const
    {
        if (!open()) { return false; }
        return !util::await([] { return !open(); }, 1s);
    }

    void TravelMap::close()
    {
        auto start = std::chrono::system_clock::now();
        while (is_open()) {
            window::press("esc", true);
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }

            // Increased timeout to 60 seconds
            if (util::timedout(start, std::chrono::seconds(60))) {
                throw failed_to_close(this);
            }
        }
    }

    void TravelMap::go_to(const std::string& destination, const bool wait_ready)
    {
        core::sleep_for(std::chrono::milliseconds(500));
        searchbar.search_for(destination);
        core::sleep_for(std::chrono::milliseconds(400));

        destination_button button = get_ready_destination(destination, wait_ready);
        button.select();

        if (!util::await([this]() -> bool { return can_confirm_travel(); },
                         std::chrono::seconds(10))) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        do { confirm_button.press(); } while (!util::await(
            [this]() -> bool { return !is_open(); },
            std::chrono::seconds(5)));
        searchbar.set_text_cleared();
    }
}
