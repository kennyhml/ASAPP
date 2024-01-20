#include "asapp/interfaces/baseinventory.h"
#include "asapp/core/state.h"
#include "asapp/game/controls.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"

namespace asa::interfaces
{
    BaseInventory::BaseInventory(const bool is_remote) :
        item_filter(is_remote ? 1205 : 175, 841, 552, 42),
        search_bar(is_remote ? 1207 : 177, 176, 142, 44),
        item_area(is_remote ? 1205 : 178, 239, 552, 588),
        transfer_all_button_(is_remote ? 1388 : 366, 176),
        drop_all_button_(is_remote ? 1436 : 413, 176),
        new_folder_button_(is_remote ? 1531 : 509, 176),
        auto_stack_button_(is_remote ? 1579 : 557, 176),
        folder_view_button_(is_remote ? 1652 : 629, 176), is_remote_inventory_(is_remote),
        area_(is_remote ? 1179 : 149, 94, 591, 827)
    {
        init_slots({is_remote ? 1205 : 178, 239});
    };

    bool BaseInventory::ManagementButton::is_toggled() const
    {
        static constexpr window::Color toggled_color{128, 231, 255};
        return cv::countNonZero(get_mask(area, toggled_color, 10)) > 30;
    }

    bool BaseInventory::ManagementButton::is_available() const
    {
        static constexpr window::Color base_color{0, 140, 171};
        return cv::countNonZero(get_mask(area, base_color, 10)) > 20;
    }

    bool BaseInventory::InvTabButton::is_selected() const
    {
        static constexpr window::Color selected_color{188, 244, 255};
        return countNonZero(get_mask(area, selected_color, 10)) > 100;
    }

    bool BaseInventory::InvTabButton::exists() const
    {
        static constexpr window::Color inactive_color{80, 141, 155};
        return cv::countNonZero(get_mask(area, inactive_color, 10)) > 100 ||
            is_selected();
    }

    bool BaseInventory::is_receiving_remote_inventory() const
    {
        assert_open(__func__);
        if (!is_remote_inventory_) { return false; }

        static constexpr window::Color text_color{191, 243, 255};
        return cv::countNonZero(get_mask(recv_remote_inv_area_, text_color, 25)) > 100;
    }

    void BaseInventory::receive_remote_inventory(const std::chrono::seconds timeout) const
    {
        assert_open(__func__);

        if (!util::await([this]() -> bool { return !is_receiving_remote_inventory(); },
                         timeout)) { throw ReceivingRemoteInventoryTimeoutError(this); }
    }

    bool BaseInventory::is_open() const
    {
        return window::match_template(item_filter.area,
                                      resources::interfaces::cb_arrowdown, 0.9f);
    }

    bool BaseInventory::has(items::Item& item, const bool search)
    {
        assert_open(__func__);

        if (search) {
            search_bar.search_for(item.get_name());
            Sleep(100);
        }

        // if an items query isnt ambigious, i.e when we enter the item name
        // ONLY the item can show up, just check the first slot for efficiency.
        if (search && !item.get_data().has_ambiguous_query) { return slots[0].has(item); }

        return match_template(item_area, item.get_inventory_icon(), 0.7f,
                              item.get_inventory_icon_mask());
    }

    bool BaseInventory::count_stacks(items::Item& item, int& count_out, const bool search)
    {
        assert_open(__func__);

        const auto matches = window::locate_all_template(
            item_area, item.get_inventory_icon(), 0.9f, item.get_inventory_icon_mask());

        if (matches.empty()) {
            count_out = 0;
            return true;
        }

        count_out = static_cast<int>(matches.size());
        return count_out != MAX_ITEMS_PER_PAGE;
    }

    const components::Slot* BaseInventory::find_item(items::Item& item,
                                                     const bool is_searched,
                                                     const bool search_for)
    {
        assert_open(__func__);

        if (!item.get_data().has_ambiguous_query && (is_searched || search_for)) {
            if (search_for) { search_bar.search_for(item.get_name()); }
            return slots[0].has(item) ? slots.data() : nullptr;
        }

        if (search_for) { search_bar.search_for(item.get_name()); }
        for (const components::Slot& slot : slots) {
            if (slot.has(item)) { return &slot; }
            if (slot.is_empty()) { return nullptr; }
        }
        return nullptr;
    }

    void BaseInventory::init_slots(const window::Point& origin)
    {
        // we got 6 rows where each row has 6 columns
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                // the slots are offset by 93 on the x and y axis
                const components::Slot slot(origin.x + (j * 93), origin.y + (i * 93));

                // the current row is (row index * 6) + col index, image row = 1 and
                // col = 4: (1 * 6) + 4 = 10, so we are at slot index 10 :)
                slots[(i * 6) + j] = slot;
            }
        }
    }

    void BaseInventory::popcorn(items::Item& item, const int stacks, int* stacks_dropped)
    {
        assert_open(__func__);
        int dropped = 0;
        bool searched = false;
        if (!search_bar.is_text_entered()) {
            search_bar.search_for(item.get_name());
            searched = true;
        }

        while (slots[0].has(item) && (dropped < stacks || stacks == -1)) {
            for (int i = 0; i < 4; i++) {
                select_slot(slots[i]);
                controls::key_press(settings::action_mappings::drop_item.key);
                dropped++;
            }
        }

        if (stacks_dropped) { *stacks_dropped = dropped; }
        if (searched) { search_bar.delete_search(); }
    }

    void BaseInventory::popcorn(const int num_slots)
    {
        assert_open(__func__);

        for (int slot = num_slots - 1; slot >= 0; slot--) {
            select_slot(slot);
            controls::key_press(settings::action_mappings::drop_item.key);
            Sleep(100);
        }
    }

    void BaseInventory::popcorn_all()
    {
        while (!slots[0].is_empty() && is_open()) {
            for (auto& slot : slots) {
                if (slot.is_empty()) { break; }
                window::set_mouse_pos(slot.area.get_random_location(5));
                asa::window::press(asa::settings::drop_item);
            }
        }
    }

    void BaseInventory::take_slot(const components::Slot& slot)
    {
        select_slot(slot);
        do {
            window::press(settings::transfer_item, false, std::chrono::milliseconds(15));
        }
        while (!util::await([&slot]() -> bool { return !slot.is_hovered(); },
                            std::chrono::seconds(5)));
    }

    void BaseInventory::close()
    {
        const auto start = std::chrono::system_clock::now();
        while (is_open()) {
            close_button_.press();
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotClosedError(this);
            }
        }
    }

    void BaseInventory::select_slot(const components::Slot& slot,
                                    bool hovered_check) const
    {
        assert_open(__func__);

        if (hovered_check) {
            do {
                const window::Point location = slot.area.get_random_location(5);
                window::set_mouse_pos(location);
            }
            while (!util::await([slot]() -> bool { return slot.is_hovered(); },
                                std::chrono::seconds(2)));
        }
        else { window::set_mouse_pos(slot.area.get_random_location(5)); }
    }

    void BaseInventory::drop_all()
    {
        assert_open(__func__);

        drop_all_button_.press();
        search_bar.set_text_cleared();
        Sleep(200);
        // TODO: Wait for the items to be dropped
    }

    void BaseInventory::drop_all(const items::Item& item)
    {
        search_bar.search_for(item.get_name());
        drop_all();
    }

    void BaseInventory::drop_all(const std::string& term)
    {
        search_bar.search_for(term);
        drop_all();
    }

    void BaseInventory::transfer_all(BaseInventory* receiver)
    {
        transfer_all_button_.press();
        search_bar.set_text_cleared();
        // TODO: Wait for the items to be transferred in a smart way
        // Consider whether we have the receiver available or not.
    }

    void BaseInventory::transfer_all(const items::Item& item, BaseInventory* receiver)
    {
        search_bar.search_for(item.get_name());
        core::sleep_for(std::chrono::milliseconds(50));
        transfer_all();
    }

    void BaseInventory::transfer_all(const std::string& term, BaseInventory* receiver)
    {
        search_bar.search_for(term);
        core::sleep_for(std::chrono::milliseconds(50));

        transfer_all(receiver);
    }

    void BaseInventory::transfer(items::Item& item, const int stacks,
                                 BaseInventory* receiver, const bool search)
    {
        assert_open(__func__);

        if (search) {
            search_bar.search_for(item.get_name());
            core::sleep_for(std::chrono::milliseconds(50));
        }

        int i = 0;
        while (const auto slot = find_item(item, search)) {
            if (i++ > stacks && stacks != 0) { break; }
            take_slot(*slot);
        }
    }

    void BaseInventory::transfer_rows(const items::Item& item, const int rows)
    {
        search_bar.search_for(item.get_name());

        window::post_mouse_press_at(slots[0].area.get_random_location(3), controls::LEFT);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < 6; j++) {
                window::set_mouse_pos(slots[j].area.get_random_location(5));
                window::post_press(asa::settings::transfer_item);
                core::sleep_for(std::chrono::milliseconds(250));
            }
        }
        search_bar.delete_search();
    }

    void BaseInventory::transfer_rows(const items::Item& item,
                                      const std::chrono::seconds duration)
    {
        search_bar.search_for(item.get_name());

        window::post_mouse_press_at(slots[0].area.get_random_location(3), controls::LEFT);
        const auto start = std::chrono::system_clock::now();

        while (!util::timedout(start, duration)) {
            for (int j = 0; j < 6; j++) {
                window::set_mouse_pos(slots[j].area.get_random_location(5));
                window::post_press(asa::settings::transfer_item);
                core::sleep_for(std::chrono::milliseconds(250));
            }
        }
        search_bar.delete_search();
    }


    void BaseInventory::auto_stack()
    {
        assert_open(__func__);
        auto_stack_button_.press();
    }

    void BaseInventory::make_new_folder(const std::string& folder_name)
    {
        assert(folder_name.size() > 1, "Folder name must be more than 1 character.");

        new_folder_button_.press();
        asa::core::sleep_for(std::chrono::milliseconds(500));

        util::set_clipboard(folder_name);
        controls::key_combination_press("ctrl", "v");
        controls::key_press("enter");
    }

    void BaseInventory::assert_open(std::string for_action) const
    {
        if (!util::await([this]() { return is_open(); }, std::chrono::seconds(5))) {
            throw InterfaceNotOpenError(std::move(for_action), this);
        }
    }

    std::vector<std::unique_ptr<items::Item>> BaseInventory::get_current_page_items(
        std::vector<std::string>* allowed_items,
        std::vector<items::ItemData::ItemType>* allowed_categories,
        const int num_threads) const
    {
        assert_open(__func__);

        const auto start = std::chrono::system_clock::now();
        std::cout << "[+] Getting all items of the current page...\n";
        // get the amount of slots that we need to fill, this makes multithreading the
        // determination process easier as we can assign one thread per slot from the start
        int num_slots_filled = 0;
        int folder_offset = 0;
        for (const auto& slot : slots) {
            if (slot.is_folder()) {
                folder_offset++;
                continue;
            }
            if (slot.is_empty()) { break; }
            num_slots_filled++;
        }

        std::cout << "\t[-] " << num_slots_filled << " slots to be determined...\n";
        std::vector<std::unique_ptr<items::Item>> ret(num_slots_filled);
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(
                [this, i, &ret, num_threads, num_slots_filled, folder_offset]() -> void {
                    for (int j = i; j < num_slots_filled; j += num_threads) {
                        ret[j] = slots[j + folder_offset].get_item();
                    }
                });
        }

        for (auto& thread : threads) { thread.join(); }
        std::cout << "\t[-] Finished. (" << util::get_elapsed<std::chrono::seconds>(start)
            << ")\n";
        return ret;
    }
}
