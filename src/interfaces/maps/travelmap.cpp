#include "asa/interfaces/maps/travelmap.h"
#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/interfaces/exceptions.h"

namespace asa
{
    namespace
    {
        bool open()
        {
            return window::match(embedded::text::beds, cv::Rect(283, 129, 91, 46));
        }
    }

    bool travel_map::is_open() const
    {
        if (!open()) { return false; }
        return !utility::await([] { return !open(); }, 1s);
    }

    void travel_map::close()
    {
        auto start = std::chrono::system_clock::now();
        while (is_open()) {
            window::press("esc");
            if (utility::await([this]() { return !is_open(); },
                               std::chrono::seconds(5))) {
                return;
            }

            // Increased timeout to 60 seconds
            if (utility::timedout(start, std::chrono::seconds(60))) {
                throw failed_to_close(this);
            }
        }
    }

    void travel_map::go_to(const std::string& destination, const bool wait_ready)
    {
        checked_sleep(500ms);
        searchbar.search_for(destination);
        checked_sleep(400ms);

        destination_button button = get_ready_destination(destination, wait_ready);
        button.select();

        if (!utility::await([this]() -> bool { return can_confirm_travel(); }, 10s)) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        do {
            confirm_button.press();
        } while (!utility::await([this]() -> bool { return !is_open(); }, 5s));
        searchbar.set_text_cleared();
    }
}
