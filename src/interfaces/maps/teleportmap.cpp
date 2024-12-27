#include "asa/ui/maps/teleportmap.h"
#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/ui/exceptions.h"

namespace asa
{
    bool teleport_map::is_open() const
    {
        return match(embedded::text::teleports, cv::Rect(239, 126, 176, 51));
    }

    void teleport_map::close()
    {
        const auto start = std::chrono::system_clock::now();
        while (is_open()) {
            post_press("esc");
            if (utility::await([this]() { return !is_open(); }, 5s)) {
                return;
            }

            // Increased timeout to 60 seconds
            if (utility::timedout(start, 60s)) { throw failed_to_close(this) ;}
        }
    }

    void teleport_map::set_selected_as_default()
    {
        if (!utility::await([this]() -> bool { return can_confirm_travel(); }, 10s)) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        set_default_button.press();
    }

    void teleport_map::go_to(const std::string& destination, bool wait_ready)
    {
        searchbar.search_for(destination);
        checked_sleep(std::chrono::milliseconds(400));

        destination_button button = get_ready_destination(destination, false);
        button.select();

        if (!utility::await([this]() -> bool { return can_confirm_travel(); }, 10s)) {
            throw std::exception("Tp confirmation button did not become available.");
        }

        do {
            confirm_button.press();
        } while (!utility::await([this]() -> bool { return !is_open(); }, 5s));
        searchbar.set_text_cleared();
    }

    std::vector<base_travel_map::destination_button> teleport_map::get_destinations() const
    {
        std::vector<destination_button> ret;

        for (const auto& roi: destination_slots_) {
            // create an imaginary button for now.
            const destination_button button(roi.x, roi.y);
            // button doesnt exist, end of the list reached.
            if (!button.is_ready()) { break; }
            ret.push_back(button);
        }
        return ret;
    }
}
