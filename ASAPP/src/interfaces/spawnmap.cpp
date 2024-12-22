#include "asa/interfaces/spawnmap.h"
#include "asa/core/state.h"
#include "asa/game/resources.h"
#include "asa/entities/localplayer.h"
#include "asa/util/util.h"
#include "asa/interfaces/exceptions.h"


namespace asa::interfaces
{
    bool SpawnMap::is_open() const
    {
        return match_template(regions_button.area, resources::interfaces::regions);
    }

    void SpawnMap::spawn_at(const int region_index)
    {
        regions_button.press();
        core::sleep_for(std::chrono::milliseconds(200));

        const auto& roi = destination_slots_[region_index];
        DestinationButton button(roi.x, roi.y);

        button.press();
        while (!can_confirm_travel()) {}
        confirm_button.press();
        pass_spawn_screen(false);
    }

    void SpawnMap::spawn_at(const std::string& bed, const bool wait_ready)
    {
        beds_button.press();
        core::sleep_for(std::chrono::milliseconds(200));

        searchbar.search_for(bed);
        core::sleep_for(std::chrono::milliseconds(400));

        DestinationButton button = get_ready_destination(bed, wait_ready);
        button.select();

        if (!util::await([this]() -> bool { return can_confirm_travel(); },
                         std::chrono::seconds(10))) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        do { confirm_button.press(); }
        while (!util::await([this]() -> bool { return !is_open(); },
                            std::chrono::seconds(5)));
        searchbar.set_text_cleared();

        pass_spawn_screen(false);
    }

    void SpawnMap::go_to(const std::string& destination, const bool wait_ready)
    {
        return spawn_at(destination, wait_ready);
    }

    void SpawnMap::pass_spawn_screen(const bool in)
    {
        // Wait for spawn map to open/close
        if (!util::await([this, in]() { return in == is_open(); },
                         std::chrono::seconds(30))) {
            if (in) { throw asa::interfaces::InterfaceNotOpenedError(this); }
            else { throw asa::interfaces::InterfaceNotClosedError(this); }
        }

        core::sleep_for(std::chrono::seconds(1));

        // If we're spawning we also need to wait for the spawn
        // to be successful and the animation to end
        if (!in) {
            // After spawning somewhere wait for the white background
            // indicating we have spawned
            auto start = std::chrono::system_clock::now();
            while (!entities::local_player->is_in_travel_screen()) {
                core::sleep_for(std::chrono::milliseconds(100));

                if (util::timedout(start, std::chrono::seconds(30))) {
                    throw asa::interfaces::InterfaceNotClosedError(this);
                }
            }

            // Server has respawned us. Now wait for the spawn animation to finish
            core::sleep_for(std::chrono::seconds(8));
            // Animation ended. It's safe to continue and accept input
        }
    }
}
