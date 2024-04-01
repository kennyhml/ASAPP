#pragma once
#include "baseentityinfo.h"

namespace asa::interfaces
{
    class PlayerInfo : public BaseEntityInfo
    {
    protected:
        components::Button you_{757, 124, 111, 49};

    public:
        using BaseEntityInfo::BaseEntityInfo;

        enum Slot : uint32_t
        {
            HEAD,
            TORSO,
            LEGS,
            HANDS,
            OFFHAND,
            FEET,
        };

        enum Stat : uint32_t
        {
            HEALTH,
            FOOD,
            OXYGEN,
            WATER,
            WEIGHT,
            MELEE,
            CRAFTING_SKILL,
            FORTITUDE,
            TORPIDITY
        };

        /**
         * @brief Checks whether the player info interface is open.
         *
         * @return True if the interface is open, false otherwise.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Gets the tame limit from the player info.
         *
         * @return The number of tames out on the server for the tribe.
         *
         * @remark This tame cap only includes platform dinos / rafts.
         */
        [[nodiscard]] int get_tame_limit() const { return 0; }

        /**
         * @brief Gets the crafting skill of the character.
         *
         * @return The amount of crafting skill the character has.
         */
        [[nodiscard]] int get_crafting_skill() const { return 0; }

        /**
         * @brief Gets the fortitude of the character.
         *
         * @return The amount of fortitude the character has.
         */
        [[nodiscard]] int get_fortitude() const { return 0; }
    };
}
