#pragma once
#include "basetravelmap.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class TeleportMap : public BaseTravelMap
    {
    public:
        using BaseTravelMap::BaseTravelMap;

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
        [[nodiscard]] std::vector<DestinationButton> get_destinations() const override;

        components::SearchBar searchbar{322, 942, 216, 46};
    private:
        components::Button set_default_button{1210, 948, 245, 43};
    };
}
