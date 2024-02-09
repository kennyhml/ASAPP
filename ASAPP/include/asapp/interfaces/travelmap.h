#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class TravelMap : public BaseTravelMap
    {
    public:
        using BaseTravelMap::BaseTravelMap;

        /**
         * @brief Closes the travel map interface.
         */
        void close();

        /**
         * @brief Searches the given destination and travels to the first suggested
         * target that is not on cooldown.
         *
         * @param destination The name of the travel traget.
         * @param wait_ready If true, wait for the bed to be ready - else throw.
         *
         * @throws DestinationNotFound when the given bed doesnt exist.
         * @throws DestinationNotReady when no beds are off cooldown.
         */
        void go_to(const std::string& destination, bool wait_ready = false) override;

    private:
        components::SearchBar searchbar{301, 940, 245, 51};
    };
}
