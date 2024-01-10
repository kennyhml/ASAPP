#pragma once
#include <array>
#include <iostream>
#include <asapp/util/util.h>
#include "iinterface.h"
#include "asapp/game/window.h"
#include "components/button.h"
#include "components/combobox.h"
#include "components/searchbar.h"
#include "components/slot.h"

namespace asa::interfaces
{
    inline constexpr int MAX_ITEMS_PER_PAGE = 36;

    class BaseInventory : public IInterface
    {
    public:
        explicit BaseInventory(bool is_remote);

        enum Tabs
        {
            INVENTORY,
        };

        std::array<components::Slot, 36> slots;

        components::ComboBox item_filter;
        components::SearchBar search_bar;
        window::Rect item_area;

        /**
         * @brief Gets the full area of the inventory, differs between local and remote.
         * 
         * @return A rect containing x, y, width and height of the inventories area.
         */
        [[nodiscard]] window::Rect get_area() const { return area_; }

        /**
         * @brief Checks whether this inventory is currently being received (loaded).
         *
         * @remarks Only a remote inventory has to be received, the local inventory
         * will always be available instantly.
         * 
         * @return True if the inventory is being received, false otherwise.
         */
        [[nodiscard]] bool is_receiving_remote_inventory() const;

        /**
         * @brief Checks whether the inventory is currently open.
         *
         * 
         * @return True if the inventory is open, false otherwise.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Waits to receive the remote inventory within a given duration.
         * 
         * @param timeout How long the action is allowed to take until an exception is thrown.
         * 
         * @throws ReceivingRemoteInventoryTimeoutError if the action took too long.
         */
        void receive_remote_inventory(const std::chrono::seconds timeout) const;

        /**
         * @brief Checks whether a given item is located within the inventory.
         * 
         * @param item The item to check for.
         * @param search Whether to search for the item to determine whether it's there.
         * 
         * @return True if at least one occurence of the item is found, false otherwise.
         */
        virtual bool has(items::Item& item, bool search = false);

        /**
         * @brief Counts how many stacks of a given item are located in an inventory.
         * 
         * @param item The item to count.
         * @param count_out Out parameter for the counted stacks.
         * @param search Whether to search for the item to count the stacks.
         *
         * @remarks Only up to 36 stacks can be counted (one inventory page).
         *
         * @return Whether the counted amount was smaller than the max inventory page.
         */
        virtual bool count_stacks(items::Item& item, int& count_out, bool search = false);

        /**
         * @brief Finds the slot of a given item in the inventory.
         * 
         * @param is_searched Whether the item is currently searched for.
         * @param search_for Whether to search for the item first.
         * 
         * @return A pointer to the slow that the item is located in, or nullptr if not found.
         */
        virtual const components::Slot* find_item(items::Item&, bool is_searched = false,
                                                  bool search_for = false);

        /**
         * @brief Popcorns a given amount of an item from the inventory.
         * 
         * @param item The item to popcorn.
         * @param stacks The amount of stacks to popcorn, default all (-1).
         * @param stacks_dropped Optional integer pointer for how many stacks were dropped.
         */
        virtual void popcorn(items::Item& item, int stacks = -1,
                             int* stacks_dropped = nullptr);

        /**
         * @brief Popcorns n-slots from the inventory.
         * \param num_slots How many slots to popcorn.
         */
        virtual void popcorn(int num_slots);

        /**
         * @brief Takes the item located at a given slot.
         * 
         * @param slot The slot to take the item from.
         */
        virtual void take_slot(const components::Slot& slot);

        /**
         * @brief Takes the item located at a given slot.
         * 
         * @param index The index of the slot to take the item from.
         */
        virtual void take_slot(const int index) { return take_slot(slots[index]); }

        /**
         * @brief Selects the given slot.
         * 
         * @param slot The slot to select.
         */
        void select_slot(const components::Slot& slot) const;

        /**
          * @brief Selects the given slot by index.
          * 
          * @index The index of the slot to select.
          */
        void select_slot(const int index) const { return select_slot(slots[index]); }

        /**
         * @brief Closes the inventory.
         *
         * @remarks Uses the x button instead of the esc key in order to prevent
         * a double press in extreme cases (game froze, bad fps, bad timing...)
         */
        void close();

        /**
         * @brief Drops all items frop the inventory.
         */
        void drop_all();

        /**
         * @brief Drops all of the given item from the inventory. 
         * @param item The item to drop all of.
         */
        void drop_all(const items::Item& item);

        /**
         * @brief Transfers all items to another inventory.
         * 
         * @param receiver A pointer to the receiving inventory (optional).
         *
         * @remarks The pointer to the receiving end may help to confirm transfers.
         */
        void transfer_all(BaseInventory* receiver = nullptr);

        /**
         * @brief Transfers all of a given item into another inventory.
         * 
         * @param item The item to transfer all of.
         * @param receiver A pointer to the receiving inventory (optional).
         * 
         * @remarks The pointer to the receiving end may help to confirm transfers.
         */
        void transfer_all(const items::Item& item, BaseInventory* receiver = nullptr);

        /**
         * @brief Transfers all of a given item into another inventory.
         * 
         * @param term The term to search and transfer all of.
         * @param receiver A pointer to the receiving inventory (optional).
         * 
         * @remarks The pointer to the receiving end may help to confirm transfers.
         */
        void transfer_all(const std::string& term, BaseInventory* receiver = nullptr);

        /**
         * @brief Transfers n-items into another inventory.
         * 
         * @param item The item to transfer.
         * @param stacks The amount of stacks to transfer.
         * @param receiver The receiving end (default nullptr).
         * @param search Whether the item should be searched for (default true).
         *
         * @remarks The pointer to the receiving end may help to confirm transfers.
         */
        void transfer(items::Item& item, int stacks = 1,
                      BaseInventory* receiver = nullptr, bool search = true);

        /**
         * @brief Transfers a given amount of rows of a given item.
         * 
         * @param item The item to transfer x rows of, will be searched beforehand 
         * @param rows The estimated amounts of rows to transfer.
         *
         * @remark The amount of rows transferred is not checked but guessed.
         */
        void transfer_rows(const items::Item& item, int rows);

        /**
         * @brief Transfers rows of a given item for a given duration.
         * 
         * @param item The item to transfer x rows of, will be searched beforehand 
         * @param duration The duration to transfer rows for in seconds.
         *
         * @remark The amount of rows transferred is not checked but guessed.
         */
        void transfer_rows(const items::Item& item, std::chrono::seconds duration);

        /**
         * @brief Sets the item filter
         */
        void set_filter();

        /**
         * @brief Creates a new folder given a name.
         * 
         * @param folder_name The name of the folder to create.
         *
         * @remarks Folder names must contain at least 2 characters.
         */
        void make_new_folder(const std::string& folder_name);

        /**
         * @brief Auto stack all items in the inventory.
         */
        void auto_stack();

        /**
         * @brief Toggles the folder view in the inventory.
         */
        void toggle_folder_view();

        /**
         * @brief Determines all items in the given slots.
         * 
         * @tparam Size The size of the array i.e the amount of slots to get the item of. 
         * @param start_index The slot index to start getting the items at.
         * @param allowed_items Whitelist of allowed items, other items are not checked.
         * @param allowed_categories Whitelist of allowed item types, others are not checked.
         * @param num_threads The number of threads to use, default 5.
         * 
         * @return An array of the given size containing smart pointers to the items, or null.
         */
        template <std::size_t Size>
        [[nodiscard]] std::array<std::unique_ptr<items::Item>, Size> get_items(
            const int start_index = 0, std::vector<std::string>* allowed_items = nullptr,
            std::vector<items::ItemData::ItemType>* allowed_categories = nullptr,
            int num_threads = 5) const
        {
            assert_open(__func__);
            const auto start = std::chrono::system_clock::now();
            std::cout << "[+] Getting items from " << start_index << " to " << start_index
                + Size << "\n";

            std::array<std::unique_ptr<items::Item>, Size> ret;
            std::vector<std::thread> threads;
            threads.reserve(num_threads);

            for (int i = 0; i < num_threads; i++) {
                threads.emplace_back([this, i, &ret, num_threads, start_index]() -> void {
                    for (int j = i; j < Size; j += num_threads) {
                        if (j > Size) { break; }
                        ret[j] = std::move(slots[j + start_index].get_item());
                    }
                });
            }

            for (auto& thread : threads) { thread.join(); }
            std::cout << "\t[-] Finished. (" << util::get_elapsed<
                std::chrono::seconds>(start) << ")\n";
            return ret;
        }

        /**
         * @brief Retrieves all items from the currently visible page.
         *
         * @param allowed_items Whitelist of allowed items, other items are not checked.
         * @param allowed_categories Whitelist of allowed item types, others are not checked.
         * @param num_threads The number of threads to use, default 5.
         * 
         * @return A vector containing unique pointers to all items  in the current page.
         *
         * @remarks When an empty slot is encountered, the evaluation is stopped and the
         * result is returned. Otherwise the result is returned after the 36th slot.
         */
        [[nodiscard]] std::vector<std::unique_ptr<items::Item>> get_current_page_items(
            std::vector<std::string>* allowed_items = nullptr,
            std::vector<items::ItemData::ItemType>* allowed_categories = nullptr,
            int num_threads = 5) const;

    protected:
        struct ManagementButton : components::Button
        {
            ManagementButton(const int t_x, const int t_y) : Button(t_x, t_y, 45, 45) {}

            [[nodiscard]] bool is_toggled() const;
            [[nodiscard]] bool is_available() const;
        };

        struct InvTabButton : components::Button
        {
            using Button::Button;

            [[nodiscard]] bool is_selected() const;
            [[nodiscard]] bool exists() const;
        };

        ManagementButton transfer_all_button_;
        ManagementButton drop_all_button_;
        ManagementButton new_folder_button_;
        ManagementButton auto_stack_button_;
        ManagementButton folder_view_button_;

        bool is_remote_inventory_;
        window::Rect area_;
        window::Rect recv_remote_inv_area_{1340, 511, 295, 34};
        components::Button close_button_{1781, 49, 36, 33};

        void assert_open(std::string for_action) const;

    private:
        void init_slots(const window::Point& origin);
    };
}
