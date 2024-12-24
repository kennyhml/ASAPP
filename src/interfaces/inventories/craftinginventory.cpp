#include "asa/interfaces/inventories/craftinginventory.h"
#include "asa/core/state.h"
#include "asa/utility.h"

namespace asa
{
    bool crafting_inventory::is_crafting() const
    {
        assert_open(__func__);
        static constexpr cv::Vec3b text{154, 229, 250};

        return utility::count_matches(cancel_queue_button_.area, text, 20) > 100;
    }

    bool crafting_inventory::is_tab_selected(const Tab tab) const
    {
        assert_open(__func__);
        static constexpr cv::Vec3b underline{255, 255, 255};

        const auto& button = (tab == INVENTORY) ? inventory_button_ : crafting_button_;
        return utility::count_matches(button.area, underline, 20) > 70;
    }

    crafting_inventory::Tab crafting_inventory::get_current_tab() const
    {
        return is_tab_selected(Tab::INVENTORY) ? Tab::INVENTORY : Tab::CRAFTING;
    }

    void crafting_inventory::cancel_craft()
    {
        assert_open(__func__);
        while (is_crafting()) {
            cancel_queue_button_.press();

            if (utility::await([this]() -> bool { return is_crafting(); }, 5s)) {
                return;
            }
        }
    }

    void crafting_inventory::switch_to(const Tab tab)
    {
        assert_open(__func__);
        if (is_tab_selected(tab)) { return; }

        auto& button = tab == INVENTORY ? inventory_button_ : crafting_button_;
        do {
            button.press();
        } while (!utility::await([this, tab] { return is_tab_selected(tab); }, 5s));
    }


    void crafting_inventory::craft(const item& item, const int amount)
    {
        assert_open(__func__);
        switch_to(CRAFTING);

        search_bar.search_for(item.get_name());
        select_slot(0);
        queue(amount);
    }

    void crafting_inventory::craft(const item_slot at_slot, const int amount)
    {
        assert_open(__func__);
        switch_to(CRAFTING);
        select_slot(at_slot);
        queue(amount);
    }

    void crafting_inventory::craft(const int slot_index, const int amount)
    {
        return craft(slots[slot_index], amount);
    }

    void crafting_inventory::queue(int amount)
    {
        // avoid setting a last craft timestamp if we didnt craft.
        if (!amount) { return; }

        while (amount >= 100) {
            window::press("a");
            amount -= 100;
            checked_sleep(50ms);
        }

        while (amount) {
            window::press("e");
            amount--;
            checked_sleep(50ms);
        }

        last_craft_ = std::chrono::system_clock::now();
    }
}
