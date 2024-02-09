#include <iostream>
#include "asapp/interfaces/travelmap.h"
#include "asapp/util/util.h"
#include "asapp/core/state.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
    void TravelMap::close()
    {
        auto start = std::chrono::system_clock::now();
        while (is_open()) {
            window::press("esc", true);
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotClosedError(this);
            }
        }
    }

    void TravelMap::go_to(const std::string& destination, const bool wait_ready)
    {
        searchbar.search_for(destination);
        core::sleep_for(std::chrono::milliseconds(400));

        DestinationButton button = get_ready_destination(destination, wait_ready);
        button.select();

        if (!util::await([this]() -> bool { return can_confirm_travel(); },
                         std::chrono::seconds(10))) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        do { confirm_button.press(); }
        while (!util::await([this]() -> bool { return !is_open(); },
                            std::chrono::seconds(5)));
        searchbar.set_text_cleared();
    }
}
