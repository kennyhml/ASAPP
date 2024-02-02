#include "asapp/entities/localplayer.h"
#include "asapp/interfaces/hud.h"
#include "asapp/util/util.h"
#include "asapp/core/state.h"
#include "asapp/entities/exceptions.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/structures/exceptions.h"
#include <iostream>


namespace asa::entities
{

    interfaces::LocalInventory* LocalPlayer::get_inventory() const
    {
        return dynamic_cast<interfaces::LocalInventory*>(inventory_.get());
    }

    bool LocalPlayer::is_alive() const
    {
        interfaces::hud->toggle_extended(true);
        bool result = util::await([]() {
            return interfaces::hud->extended_information_is_toggled();
        }, std::chrono::milliseconds(300));

        interfaces::hud->toggle_extended(false);
        return result;
    }

    bool LocalPlayer::is_out_of_water() const
    {
        return interfaces::hud->is_player_out_of_water();
    }

    bool LocalPlayer::is_out_of_food() const
    {
        return interfaces::hud->is_player_out_of_food();
    }

    bool LocalPlayer::is_broken_bones() const
    {
        return interfaces::hud->is_player_broken_bones();
    }

    bool LocalPlayer::is_overweight() const
    {
        return interfaces::hud->is_player_overweight();
    }

    bool LocalPlayer::received_item(items::Item& item) const
    {
        return interfaces::hud->item_added(item, nullptr);
    }

    bool LocalPlayer::deposited_item(items::Item& item) const
    {
        return interfaces::hud->item_removed(item, nullptr);
    }

    bool LocalPlayer::is_in_travel_screen() const
    {
        static window::Rect roi(94, 69, 1751, 883);

        cv::Mat image = screenshot(roi);
        cv::Mat gray;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        cv::Scalar mean, stddev;
        meanStdDev(gray, mean, stddev);
        return mean[0] > 240.f;
    }

    bool LocalPlayer::can_access_bed() const
    {
        return interfaces::hud->can_fast_travel();
    }

    bool LocalPlayer::can_access_inventory() const
    {
        return interfaces::hud->can_access_inventory();
    }

    bool LocalPlayer::can_use_default_teleport() const
    {
        return interfaces::hud->can_default_teleport();
    }

    bool LocalPlayer::deposit_into_dedi(items::Item& item, int* amount_out)
    {
        auto deposited = [this, &item, amount_out]() -> bool {
            if (amount_out) { return get_amount_removed(item, *amount_out); }
            return deposited_item(item);
        };

        const auto start = std::chrono::system_clock::now();
        do {
            window::press(settings::use);
            if (util::timedout(start, std::chrono::seconds(10))) {
                throw structures::StructureError(
                        nullptr,
                        std::format("Failed to deposit '{}' into dedicated storage.",
                                    item.get_name()));
            }
        } while (!util::await(deposited, std::chrono::seconds(5)));
        return true;
    }

    bool LocalPlayer::withdraw_from_dedi(items::Item& item, int* amount_out)
    {
        return false;
    }

    bool LocalPlayer::get_amount_added(items::Item& item, int& amount_out)
    {
        return interfaces::hud->count_items_added(item, amount_out);
    }

    bool LocalPlayer::get_amount_removed(items::Item& item, int& amount_out)
    {
        return interfaces::hud->count_items_removed(item, amount_out);
    }

    void LocalPlayer::suicide()
    {
        const auto start = std::chrono::system_clock::now();
        std::cout << "[+] Suiciding with implant...\n";
        interfaces::hud->toggle_extended(true);

        get_inventory()->open();
        if (get_inventory()->slots[0].is_empty()) {
            std::cerr << "[!] Got glitched implant, trying again..\n";
            get_inventory()->close();
            return suicide();
        }

        controls::mouse_press(controls::LEFT);
        core::sleep_for(std::chrono::milliseconds(100));
        inventory_->select_slot(0);

        std::cout << "\t[-] Waiting for implant cooldown... ";
        core::sleep_for(std::chrono::seconds(6));
        std::cout << "Done.\n";
        do {
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw SuicideFailedError();
            }
            window::press(settings::use);
            core::sleep_for(std::chrono::seconds(3));
        } while (interfaces::hud->extended_information_is_toggled());
        std::cout << "\t[-] Suicided successfully.\n";
        reset_view_angles();
        is_crouched_ = false;
        is_proned_ = false;
        interfaces::hud->toggle_extended(false, true);
    }

    void LocalPlayer::jump()
    {
        if (is_proned_ || is_crouched_) {
            is_proned_ = false;
            is_crouched_ = false;
            jump();
        }
        window::press(settings::jump);
    }

    void LocalPlayer::crouch()
    {
        if (!is_crouched_) { window::press(settings::crouch); }
        is_crouched_ = true;
        is_proned_ = false;
    }

    void LocalPlayer::prone()
    {
        if (!is_proned_) { window::press(settings::prone); }
        is_proned_ = true;
        is_crouched_ = false;
    }

    void LocalPlayer::stand_up()
    {
        if (is_proned_ || is_crouched_) { window::press(settings::run); }
        is_proned_ = false;
        is_crouched_ = false;
    }

    bool LocalPlayer::can_access(const structures::BaseStructure&) const
    {
        return interfaces::hud->can_access_inventory();
        // TODO: if ghud fails use the action wheel
    }

    bool LocalPlayer::can_access(const BaseEntity&) const
    {
        return interfaces::hud->can_access_inventory();
        // TODO: if ghud fails use the action wheel
    }

    void LocalPlayer::access(const BaseEntity& entity) const
    {
        if (entity.get_inventory()->is_open()) { return; }

        auto start = std::chrono::system_clock::now();
        do {
            window::press(settings::access_inventory, true);
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw EntityNotAccessed(&entity);
            }
        } while (!util::await([&entity]() { return entity.get_inventory()->is_open(); },
                              std::chrono::seconds(5)));

        entity.get_inventory()->receive_remote_inventory(std::chrono::seconds(30));
    }

    void LocalPlayer::access(const structures::Container& container) const
    {
        // Accessing the inventory is the same as accessing the interface of
        // any interactable structure such as teleporters, beds etc.
        // just that we have to wait to receive the remote inventory afterward.
        access(static_cast<const structures::InteractableStructure&>(container));
        container.get_inventory()->receive_remote_inventory(std::chrono::seconds(30));
    }

    void LocalPlayer::access(const structures::SimpleBed& bed,
                             const AccessFlags_ flags)
    {
        static structures::Container bag("Item Cache", 0);

        if (bed.get_interface()->is_open()) { return; }

        const bool special_access_set = (flags & AccessFlags_AccessAboveOrBelow) ==
                                        AccessFlags_AccessAboveOrBelow;

        for (int attempt = 0; attempt < 3; attempt++) {
            if (!special_access_set) {
                handle_access_direction(flags);
            }

            // If a bag is seen, give it one second to disappear.
            if (!util::await([this]() -> bool { return !can_access(bag); },
                             std::chrono::seconds(1))) {
                access(bag);
                // Check health level to ensure its an item cache.
                if (bag.get_info()->get_health_level() == 0.f) {
                    bag.get_inventory()->popcorn_all();
                } else { reset_pitch(); }
                bag.get_inventory()->close();

                core::sleep_for(std::chrono::seconds(1));
                continue;
            }

            if (special_access_set) { set_pitch(90); }
            if (util::await(interfaces::HUD::can_fast_travel, std::chrono::seconds(1))) {
                break;
            }
            if (special_access_set) {
                set_pitch(-90);
                if (util::await(interfaces::HUD::can_fast_travel,
                                std::chrono::seconds(1))) {
                    break;
                }
            }

            // Still unable to see the bed, either missing or not yet loaded.
            if (flags & AccessFlags_InstantFail) {
                throw structures::StructureNotFoundError(&bed);
            }

            // TODO: Implement the action wheel as 2nd indicator we are unable to access it
            const auto timeout = std::chrono::seconds(10);
            if (!util::await(interfaces::HUD::can_fast_travel, timeout)) {
                reset_pitch();
                continue;
            }
        }
        // At this point all the specific work for beds is done, can let the
        // general access method take over the rest.
        access(static_cast<const structures::InteractableStructure&>(bed));
    }

    void LocalPlayer::access(const structures::InteractableStructure& structure) const
    {
        if (structure.get_interface()->is_open()) { return; }

        const auto start = std::chrono::system_clock::now();
        do {
            window::press(structure.get_interact_key(), true);
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw structures::StructureNotOpenedError(&structure);
            }
        } while (!util::await([&structure]() {
            return structure.get_interface()->is_open();
        }, std::chrono::seconds(5)));
    }

    void LocalPlayer::mount(const DinoEnt& entity) const
    {
        interfaces::hud->toggle_extended(true);
        asa::core::sleep_for(std::chrono::milliseconds(200));

        if (entity.is_mounted()) {
            interfaces::hud->toggle_extended(false);
            return;
        }

        do {
            if (entity.get_inventory()->is_open()) {
                entity.get_inventory()->close();
            }
            window::press(settings::use);
        } while (!util::await([&entity]() -> bool { return entity.is_mounted(); },
                              std::chrono::seconds(5)));

        interfaces::hud->toggle_extended(false);
    }

    void LocalPlayer::fast_travel_to(const structures::SimpleBed& bed,
                                     const AccessFlags_ access_flags,
                                     const TravelFlags_ travel_flags)
    {
        try {
            access(bed, access_flags);
        } catch (const std::exception& e) {
            throw FastTravelFailedError(bed.get_name(), e.what());
        }

        bed.get_interface()->go_to(bed.get_name());

        // always wait for the animation to start, dont wait for it to end if
        // the no travel animation flag is set.
        pass_travel_screen(true, !(travel_flags & TravelFlags_NoTravelAnimation));
        reset_view_angles();
        is_crouched_ = false;
        is_proned_ = false;
    }

    void LocalPlayer::teleport_to(const structures::Teleporter& tp, const bool is_default)
    {
        const bool could_access_before = can_access(tp);
        if (!is_default) {
            look_fully_down();
            access(tp);
            core::sleep_for(std::chrono::milliseconds(500));
            tp.get_interface()->go_to(tp.get_name());
            util::await([]() { return !interfaces::hud->can_default_teleport(); },
                        std::chrono::seconds(5));
        } else {
            do { window::press(settings::reload); }
            while (!util::await([]() { return !interfaces::hud->can_default_teleport(); },
                                std::chrono::seconds(5)));
        }
        pass_teleport_screen(!could_access_before);
    }

    void LocalPlayer::get_off_bed()
    {
        window::press(settings::use);
        core::sleep_for(std::chrono::seconds(3));
        reset_view_angles();
    }

    void LocalPlayer::set_yaw(const int yaw)
    {
        const int diff = ((yaw - current_yaw_) + 180) % 360 - 180;
        diff < 0 ? turn_left(-diff) : turn_right(diff);
        current_yaw_ = yaw;
    }

    void LocalPlayer::set_pitch(const int pitch)
    {
        const int diff = ((pitch - current_pitch_) + 90) % 360 - 90;
        diff < 0 ? turn_up(-diff) : turn_down(diff);
        current_pitch_ = pitch;
    }

    void LocalPlayer::turn_right(const int by_degrees, const ms delay)
    {
        controls::turn_degrees(by_degrees, 0);
        current_yaw_ += by_degrees;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_left(const int by_degrees, const ms delay)
    {
        controls::turn_degrees(-by_degrees, 0);
        current_yaw_ -= by_degrees;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_down(const int by_degrees, const ms delay)
    {
        const int allowed = std::min(90 - current_pitch_, by_degrees);
        controls::turn_degrees(0, allowed);
        current_pitch_ += allowed;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_up(const int by_degrees, const ms delay)
    {
        const int allowed = std::min(90 + current_pitch_, by_degrees);
        controls::turn_degrees(0, -allowed);
        current_pitch_ -= allowed;
        core::sleep_for(delay);
    }

    void LocalPlayer::equip(items::Item* item, interfaces::PlayerInfo::Slot slot)
    {
        const bool was_inventory_open = inventory_->is_open();
        if (!was_inventory_open) {
            get_inventory()->open();
            core::sleep_for(std::chrono::milliseconds(500));
        }

        get_inventory()->equip(*item, slot);
        if (!was_inventory_open) { get_inventory()->close(); }
    }

    void LocalPlayer::unequip(interfaces::PlayerInfo::Slot slot)
    {
        bool was_inventory_open = get_inventory()->is_open();
        if (!was_inventory_open) {
            get_inventory()->open();
            core::sleep_for(std::chrono::milliseconds(500));
        }
        get_inventory()->info.unequip(slot);
        if (!was_inventory_open) { get_inventory()->close(); }
    }

    void LocalPlayer::pass_travel_screen(const bool in, const bool out)
    {
        if (in) {
            if (!util::await([this]() { return is_in_travel_screen(); },
                             std::chrono::seconds(30))) {}
        }

        if (out) {
            if (!util::await([this]() { return !is_in_travel_screen(); },
                             std::chrono::seconds(30))) {}
        }

        core::sleep_for(std::chrono::seconds(1));
    }

    void LocalPlayer::pass_teleport_screen(const bool access_flag)
    {
        while (!interfaces::hud->can_default_teleport()) {
            // for long distance teleports we still enter a white screen,
            // so we can simply reuse our bed logic
            if (is_in_travel_screen()) {
                std::cout << "[+] Whitescreen entered upon teleport." << std::endl;
                return pass_travel_screen(false);
            }
            if (access_flag && can_access_inventory()) {
                std::cout << "[+] Teleported to a container." << std::endl;
                return;
            }
        }
        // See whether the default teleport popup lasts for more than 500ms
        // if it doesnt its a glitched popup that appears when the teleport has
        // happened. Restart the procedure in that case
        if (util::await([]() { return !interfaces::hud->can_default_teleport(); },
                        std::chrono::milliseconds(500))) {
            std::cout << "[!] Glitched default teleport popup found." << std::endl;
            return pass_teleport_screen();
        }
    }

    void LocalPlayer::look_fully_down()
    {
        for (int i = 0; i < 10; i++) { turn_down(18, std::chrono::milliseconds(20)); }
        core::sleep_for(std::chrono::milliseconds(300));
    }

    void LocalPlayer::look_fully_up()
    {
        for (int i = 0; i < 10; i++) { turn_up(18, std::chrono::milliseconds(20)); }
        core::sleep_for(std::chrono::milliseconds(300));
    }

    void LocalPlayer::handle_access_direction(const AccessFlags_ flags)
    {
        if (flags & AccessFlags_AccessBelow) { set_pitch(90); }
        else if (flags & AccessFlags_AccessAbove) { set_pitch(-90); }
        if (flags & AccessFlags_AccessLeft) { set_yaw(-90); }
        else if (flags & AccessFlags_AccessRight) { set_yaw(90); }
    }


}
