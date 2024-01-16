#include "asapp/interfaces/craftinginventory.h"


namespace asa::interfaces
{
    bool CraftingInventory::is_crafting() const
    {
        assert_open(__func__);
        static constexpr window::Color text{154, 229, 250};

        const cv::Mat mask = window::get_mask(cancel_queue_button_.area, text, 20);
        return cv::countNonZero(mask) > 100;
    }

    bool CraftingInventory::is_tab_selected(const Tab tab) const
    {
        assert_open(__func__);
        static constexpr window::Color underline{255, 255, 255};

        auto& button = (tab == Tab::INVENTORY) ? inventory_button_ : crafting_button_;
        const cv::Mat mask = window::get_mask(button.area, underline, 20);
        return cv::countNonZero(mask) > 70;
    }

    CraftingInventory::Tab CraftingInventory::get_current_tab() const
    {
        return is_tab_selected(Tab::INVENTORY) ? Tab::INVENTORY : Tab::CRAFTING;
    }

    void CraftingInventory::cancel_craft()
    {
        assert_open(__func__);
        while (is_crafting()) {
            cancel_queue_button_.press();

            if (util::await([this]() -> bool { return is_crafting(); },
                            std::chrono::seconds(5))) { return; }
        }
    }

    void CraftingInventory::switch_to(const Tab tab)
    {
        assert_open(__func__);
        if (is_tab_selected(tab)) { return; }

        auto& button = tab == Tab::INVENTORY ? inventory_button_ : crafting_button_;

        do { button.press(); }
        while (!util::await([this, tab]() -> bool { return is_tab_selected(tab); },
                            std::chrono::seconds(5)));
    }


    void CraftingInventory::craft(const items::Item& item, const int amount)
    {
        assert_open(__func__);
        switch_to(Tab::CRAFTING);

        search_bar.search_for(item.get_name());
        select_slot(0);
        queue(amount);
    }

    void CraftingInventory::craft(const components::Slot at_slot, const int amount)
    {
        assert_open(__func__);
        switch_to(Tab::CRAFTING);
        select_slot(at_slot);
        queue(amount);
    }

    void CraftingInventory::craft(const int slot_index, const int amount)
    {
        return craft(slots[slot_index], amount);
    }

    void CraftingInventory::queue(int amount)
    {
        // avoid setting a last craft timestamp if we didnt craft.
        if (!amount) { return; }

        while (amount > 100) {
            window::press("a");
            amount -= 100;
        }

        while (amount) {
            window::press("e");
            amount--;
        }

        last_craft_ = std::chrono::system_clock::now();
    }
}
