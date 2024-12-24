#pragma once
#include "basetravelmap.h"
#include "asa/interfaces/components/search_bar.h"

namespace asa
{
    class teleport_map : public base_travel_map
    {
    public:
        using base_travel_map::base_travel_map;

        bool is_open() const override;

        /**
         * @brief Closes the teleport map interface.
         */
        void close();

        /**
         * @brief Marks the selected destination as default destination.
         */
        void set_selected_as_default();

        /**
         * @brief Teleports to the given destination.
         *
         * @param destination The name of the destination to search & travel to.
         * @param wait_ready Can be ignored because as of now, tps have no cooldown.
         */
        void go_to(const std::string& destination, bool wait_ready = false) override;

        /**
         * @brief Gets the available destination buttons.
         */
        [[nodiscard]] std::vector<destination_button> get_destinations() const override;

        search_bar searchbar{322, 942, 216, 46};
    private:
        button set_default_button{1210, 948, 245, 43};
    };
}
