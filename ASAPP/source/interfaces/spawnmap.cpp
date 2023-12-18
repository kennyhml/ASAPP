#include "asapp/interfaces/spawnmap.h"
#include "asapp/core/state.h"
#include "asapp/game/resources.h"


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
    }

    void SpawnMap::go_to(const std::string& dest) { return spawn_at(dest); }
}
