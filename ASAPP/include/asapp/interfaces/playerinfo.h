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

        enum Slot
        {
            HEAD,
            TORSO,
            LEGS,
            HANDS,
            OFFHAND,
            FEET,
        };

        enum Stat
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

        /**
         * @brief Gets the durability of an equipped piece of armor.
         * 
         * @return The amount of durability the gear in the slot has.
         */
        [[nodiscard]] bool get_gear_durability(const Slot slot, int& dura_out)
        {
            return false;
        }

        /**
         * @brief Gets the armor of an equipped piece of armor.
         * 
         * @return The amount of armor the gear in the slot has.
         */
        [[nodiscard]] bool get_gear_armor(const Slot slot, int& armor_out)
        {
            return false;
        }

        /**
         * @brief Checks if the player has a given piece of gear equipped in the given slot.
         * 
         * @return True if the item is equipped in the slot, false otherwise.
         */
        [[nodiscard]] bool has_equipped(items::Item& item, const Slot slot) const;

        /**
         * @brief Checks whether the player has any piece of gear equipped in a given slot.
         * 
         * @return True if any item is equipped in the slot, false otherwise.
         */
        [[nodiscard]] bool slot_empty(const Slot slot) const;

        /**
         * @brief Unequips the item in a given slot.
         */
        void unequip(const Slot slot);

        /**
         * @brief Unequips all items.
         */
        void unequip_all();

        /**
         * @brief Determines the item located in the given equipment slot.
         * 
         * @param slot The slot to determine the item of.
         * 
         * @return A unique pointer to the determined item or nullptr if not found.
         */
        std::unique_ptr<items::Item> get_equipped_item(Slot slot) const;
    };
}
