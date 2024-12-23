#pragma once
#include "baseinventory.h"


namespace asa
{
    class crafting_inventory : public base_inventory
    {
    public:
        enum Tab
        {
            INVENTORY,
            CRAFTING
        };

    public:
        crafting_inventory() : base_inventory(true) {}

        /**
         * @brief Checks whether something is currently being crafted. 
         */
        [[nodiscard]] bool is_crafting() const;

        /**
         * @brief Checks whether a given tab is currently selected.
         */
        [[nodiscard]] bool is_tab_selected(Tab) const;

        /**
         * @brief Gets the currently open tab 
         */
        [[nodiscard]] Tab get_current_tab() const;

        /**
         * @brief Switches to the given inventory tab, either inventory or crafting.
         */
        void switch_to(Tab);

        /**
         * @brief Cancels the crafting queue in the structure.
         */
        void cancel_craft();

        /**
         * @brief Crafts a given amount of a given item.
         * 
         * @param item The item to craft.
         * @param amount The amount of crafts to queue, must be between 1 and 1000.
         */
        void craft(const item& item, int amount);

        /**
         * @brief Queues a given amount of crafts at a given slot.
         * 
         * @param at_slot The slot to queue the item at.
         * @param amount The amount of crafts to queue, must be between 1 and 1000.
         */
        void craft(item_slot at_slot, int amount);

        /**
         * @brief Queues a given amount of crafts at a given slot index.
         * 
         * @param slot_index The index of the slot to queue the item at.
         * @param amount The amount of crafts to queue, must be between 1 and 1000.
         */
        void craft(int slot_index, int amount);

    private:
        void queue(int amount);

        std::chrono::system_clock::time_point last_craft_;

        button cancel_queue_button_{1385, 694, 186, 30};
        button inventory_button_{1207, 118, 271, 48};
        button crafting_button_{1478, 116, 273, 50};
    };
}
