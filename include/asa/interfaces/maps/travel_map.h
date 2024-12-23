#pragma once
#include "basetravelmap.h"
#include "asa/interfaces/components/search_bar.h"

namespace asa
{
    class travel_map : public base_travel_map
    {
    public:
        using base_travel_map::base_travel_map;

        bool is_open() const override;

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
         * @throws destination_not_found when the given bed doesnt exist.
         * @throws destination_not_ready when no beds are off cooldown.
         */
        void go_to(const std::string& destination, bool wait_ready = false) override;

    private:
        search_bar searchbar{322, 942, 216, 46};
    };
}
