#include "asapp/interfaces/spawnmap.h"
#include "asapp/core/state.h"
#include "asapp/game/resources.h"
#include "asapp/entities/localplayer.h"
#include "asapp/util/util.h"
#include "asapp/interfaces/exceptions.h"


namespace asa::interfaces
{
    bool SpawnMap::is_open() const
    {
        return match_template(regions_button.area, resources::interfaces::regions);
    }

    void SpawnMap::spawn_at(int regionIndex)
    {
        regions_button.press();
        core::sleep_for(std::chrono::milliseconds(200));

        results[regionIndex].press();
        while (!can_confirm_target()) {}
        confirm_button.press();
        pass_spawn_screen(false); // make sure we are spawning
    }

    void SpawnMap::spawn_at(const std::string& bed)
    {
        beds_button.press();
        core::sleep_for(std::chrono::milliseconds(200));

        searchbar.search_for(bed);
        core::sleep_for(std::chrono::milliseconds(400));
        select_result();

        while (!can_confirm_target()) {}
        confirm_button.press();
        searchbar.set_text_cleared();
        
        // Wait until we're no longer in the spawn map
        pass_spawn_screen(false);
    }

    void SpawnMap::go_to(const std::string& dest) { return spawn_at(dest); }

    // Wait for the spawn map to open (with in set to true) or to close (in set to false) and
    // the respawn to finish
    void SpawnMap::pass_spawn_screen(bool in) 
    {
        // Wait for spawn map to open/close
        if (!util::await([this, in]() { 
            auto is_open = this->is_open();
            return in == is_open;
        }, std::chrono::seconds(30))) {
            if (in) {
                throw asa::interfaces::InterfaceNotOpenedError(this);
            } else {
                throw asa::interfaces::InterfaceNotClosedError(this);
            }
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
            core::sleep_for(std::chrono::seconds(10));
  
            // Animation ended. It's safe to continue and accept input
        }
    }
}
