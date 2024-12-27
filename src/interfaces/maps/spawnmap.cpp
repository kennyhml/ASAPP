#include "asa/ui/maps/spawnmap.h"

#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/entities/localplayer.h"
#include "asa/ui/exceptions.h"

namespace asa
{
    bool spawn_map::is_open() const
    {
        return match(embedded::interfaces::regions, regions_button.area);
    }

    void spawn_map::spawn_at(const int region_index)
    {
        regions_button.press();
        checked_sleep(std::chrono::milliseconds(200));

        const auto& roi = destination_slots_[region_index];
        destination_button button(roi.x, roi.y);

        button.press();
        while (!can_confirm_travel()) {}
        confirm_button.press();
        pass_spawn_screen(false);
    }

    void spawn_map::spawn_at(const std::string& bed, const bool wait_ready)
    {
        beds_button.press();
        checked_sleep(std::chrono::milliseconds(200));

        searchbar.search_for(bed);
        checked_sleep(std::chrono::milliseconds(400));

        destination_button button = get_ready_destination(bed, wait_ready);
        button.select();

        if (!utility::await([this]() -> bool { return can_confirm_travel(); }, 10s)) {
            throw std::exception("Travel confirmation button did not become available.");
        }

        do { confirm_button.press(); } while (!utility::await(
            [this]() -> bool { return !is_open(); },
            std::chrono::seconds(5)));
        searchbar.set_text_cleared();

        pass_spawn_screen(false);
    }

    void spawn_map::go_to(const std::string& destination, const bool wait_ready)
    {
        return spawn_at(destination, wait_ready);
    }

    void spawn_map::pass_spawn_screen(const bool in)
    {
        // Wait for spawn map to open/close
        if (!utility::await([this, in]() { return in == is_open(); },
                            std::chrono::seconds(30))) {
            if (in) { throw failed_to_open(this); }
            throw failed_to_close(this);
        }

        checked_sleep(std::chrono::seconds(1));

        // If we're spawning we also need to wait for the spawn
        // to be successful and the animation to end
        if (!in) {
            // After spawning somewhere wait for the white background
            // indicating we have spawned
            auto start = std::chrono::system_clock::now();
            while (!get_local_player()->is_in_travel_screen()) {
                checked_sleep(100ms);

                if (utility::timedout(start, 30s)) { throw failed_to_close(this); }
            }

            // Server has respawned us. Now wait for the spawn animation to finish
            checked_sleep(8s);
        }
    }

    std::shared_ptr<spawn_map> get_spawn_map()
    {
        return std::make_shared<spawn_map>();
    }
}
