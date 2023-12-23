#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class SpawnMap : public BaseTravelMap
    {
        using BaseTravelMap::BaseTravelMap;

    private:
        void pass_spawn_screen(bool in = true);
      
        components::SearchBar searchbar{93, 941, 240, 50};

        components::Button new_survivor_button{1210, 948, 244, 42};
        components::Button regions_button{176, 129, 176, 52};
        components::Button beds_button{351, 132, 129, 48};

    public:
        bool is_open() const override;

        void spawn_at(int region_index = 0);
        void spawn_at(const std::string& bed);

        void go_to(const std::string& destination) override;
    };


    inline std::unique_ptr<SpawnMap> spawn_map = std::make_unique<SpawnMap>();
}
