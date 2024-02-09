#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class SpawnMap final : public BaseTravelMap
    {
    public:
        using BaseTravelMap::BaseTravelMap;

        /**
         * @brief Checks whether the spawn map is open.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Spawns at a given region index.
         *
         * @param region_index The index of the region to spawn at.
         */
        void spawn_at(int region_index = 0);

        /**
         * @brief Spawns at a bed given its name.
         *
         * @param bed The bed to spawn at, the first ready target is chosen.
         */
        void spawn_at(const std::string& bed, bool wait_ready = false);

        void go_to(const std::string& destination, bool wait_ready = false) override;

    private:
        void pass_spawn_screen(bool in = true);

        components::SearchBar searchbar{93, 941, 240, 50};

        components::Button new_survivor_button{1210, 948, 244, 42};
        components::Button regions_button{176, 129, 176, 52};
        components::Button beds_button{351, 132, 129, 48};
    };


    inline std::unique_ptr<SpawnMap> spawn_map = std::make_unique<SpawnMap>();
}
