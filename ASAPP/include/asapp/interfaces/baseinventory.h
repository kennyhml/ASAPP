#pragma once
#include <array>
#include "iinterface.h"
#include "asapp/game/window.h"
#include "asapp/items/items.h"
#include "components/button.h"
#include "components/combobox.h"
#include "components/searchbar.h"
#include "components/slot.h"

namespace asa::interfaces
{
    inline constexpr int MAX_ITEMS_PER_PAGE = 36;

    class BaseInventory : public IInterface
    {
    protected:
        struct ManagementButton : components::Button
        {
            ManagementButton(int t_x, int t_y) : Button(t_x, t_y, 45, 45) {};

            [[nodiscard]] bool is_toggled() const;
            [[nodiscard]] bool is_available() const;
        };

        struct InvTabButton : components::Button
        {
            using Button::Button;

            [[nodiscard]] bool is_selected() const;
            [[nodiscard]] bool exists() const;
        };

        ManagementButton transfer_all_button;
        ManagementButton drop_all_button;
        ManagementButton new_folder_button;
        ManagementButton auto_stack_button;
        ManagementButton folder_view_button;

        bool is_remote_inventory;
        window::Rect area;
        window::Rect recv_remote_inventory_area{1340, 511, 295, 34};

    public:
        BaseInventory(bool is_remote);

        enum Tab
        {
            INVENTORY,
        };

        components::ComboBox item_filter;
        components::SearchBar search_bar;

        window::Rect item_area;

        std::array<components::Slot, 36> slots;

        window::Rect get_area() const;

        bool is_receiving_remote_inventory() const;
        void receive_remote_inventory(std::chrono::seconds timeout) const;

        bool is_open() const override;
        virtual bool has(items::Item* item, bool search = false);
        virtual bool count_stacks(items::Item* item, int& stacks_out,
                                  bool search = false);

        virtual const components::Slot* find_item(items::Item*, bool is_searched = false,
                                                  bool search_for = false);

        virtual void popcorn(items::Item* item);
        virtual void popcorn(items::Item* item, int stacks);
        virtual void popcorn(items::Item* item, int stacks, int& stacks_dropped);
        virtual void popcorn_slots(int slots);

        virtual void take_slot(components::Slot slot);
        virtual void take_slot(int index);

        void close();

        void select_slot(components::Slot slot);
        void select_slot(int index);

        void drop_all();
        void drop_all(items::Item* item);

        void transfer_all(items::Item* = nullptr, BaseInventory* tar = nullptr);
        void transfer_all(const std::string& term, BaseInventory* tar = nullptr);

        void transfer(items::Item*, int amount = 1, BaseInventory* = nullptr,
                      bool search = true);

        void set_filter();
        void make_new_folder(std::string folder_name);
        void auto_stack();
        void toggle_folder_view();

    private:
        void init_slots(const window::Point& origin);
    };
}
