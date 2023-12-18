#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class TravelMap : public BaseTravelMap
    {
        using BaseTravelMap::BaseTravelMap;

    private:
        components::SearchBar searchbar{301, 940, 245, 51};

    public:
        void close();
        void go_to(const std::string& destination) override;
    };

    inline std::unique_ptr<TravelMap> travel_map = std::make_unique<TravelMap>();
}
