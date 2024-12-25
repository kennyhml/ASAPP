#pragma once
#include "basetravelmap.h"
#include "asa/interfaces/components/search_bar.h"

namespace asa
{
    class spawn_map final : public base_travel_map
    {
    public:
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
        using base_travel_map::base_travel_map;

        friend std::shared_ptr<spawn_map> get_spawn_map();

        void pass_spawn_screen(bool in = true);

        search_bar searchbar{93, 941, 240, 50};

        button new_survivor_button{1210, 948, 244, 42};
        button regions_button{176, 129, 176, 52};
        button beds_button{351, 132, 129, 48};
    };

    std::shared_ptr<spawn_map> get_spawn_map();
}
