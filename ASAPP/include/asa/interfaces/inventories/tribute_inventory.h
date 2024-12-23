#pragma once
#include "baseinventory.h"

namespace asa
{
    class tribute_inventory : public base_inventory
    {
    public:
        enum Tab
        {
            INVENTORY,
            CREATURES,
            CRAFTING
        };

        tribute_inventory();

        /**
         * @brief Switches to the given inventory tab, either inventory or crafting.
         */
        void switch_to(Tab);

        /**
         * @brief Starts a bossfight given the name of the boss summon, for example:
         * - alpha broodmother
         * - beta dragon
         * - gamma megapithecus
         * @param boss_name
         */
        void summon(const std::string& boss_name);
    };
}
