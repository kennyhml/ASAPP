#include "asa/entities/localplayer.h"
#include "asa/interfaces/hud.h"
#include "../../include/asa/utility.h"
#include "asa/core/state.h"
#include "asa/entities/exceptions.h"
#include "../../include/asa/game/settings.h"
#include "asa/game/window.h"
#include "asa/structures/exceptions.h"
#include "asa/game/resources.h"
#include "asa/interfaces/console.h"
#include "asa/interfaces/main_menu.h"
#include "asa/interfaces/menu.h"
#include "asa/interfaces/mode_select.h"
#include "asa/interfaces/server_select.h"
#include "asa/interfaces/spawnmap.h"
#include "asa/network/queries.h"


namespace asa::entities
{
    interfaces::LocalInventory* LocalPlayer::get_inventory() const
    {
        return dynamic_cast<interfaces::LocalInventory*>(inventory_.get());
    }

    bool LocalPlayer::is_alive() const
    {
        interfaces::hud->toggle_extended(true);
        const bool result = util::await([] {
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

    bool LocalPlayer::is_in_connect_screen() const
    {
        static window::Rect roi(94, 69, 1751, 883);

        cv::Mat image = screenshot(roi);
        cv::Mat gray;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        cv::Scalar mean, stddev;
        meanStdDev(gray, mean, stddev);
        return mean[0] < 5.f;
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
            if (util::timedout(start, std::chrono::seconds(15))) {
                throw structures::structure_error(
                    nullptr, std::format("Failed to deposit '{}' into dedicated storage.",
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

    bool LocalPlayer::turn_to_closest_waypoint(const window::Color& color,
                                               const float variance)
    {
        const cv::Mat screen = window::screenshot();
        const cv::Mat masked = window::get_mask(screen, color, variance);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(masked, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::Rect rect;
        double max_rect = 0.0;

        for (const auto& cont: contours) {
            if (const double area = cv::contourArea(cont); area > max_rect) {
                max_rect = area;
                rect = cv::boundingRect(cont);
            }
        }
        if (rect.empty() || rect.area() < 100) {
            return false;
        }

        controls::turn_to(rect.x + rect.width / 2, rect.y + rect.height / 2);
        core::sleep_for(100ms);
        return true;
    }

    void LocalPlayer::suicide()
    {
        const auto start = std::chrono::system_clock::now();
        interfaces::hud->toggle_extended(true);

        get_inventory()->open();
        if (get_inventory()->slots[0].is_empty()) {
            get_inventory()->close();
            return suicide();
        }

        core::sleep_for(std::chrono::milliseconds(100));
        inventory_->select_slot(0, true, true);
        core::sleep_for(std::chrono::seconds(6));

        do {
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw SuicideFailedError();
            }
            // right click the implant to resolve the glitched implant
            controls::mouse_press(controls::RIGHT);
            window::press(settings::use);
            core::sleep_for(std::chrono::seconds(3));
        } while (interfaces::hud->extended_information_is_toggled());
        while (!interfaces::spawn_map->is_open()) {}

        reset_state();
        interfaces::hud->toggle_extended(false, true);
    }

    void LocalPlayer::reconnect()
    {
        const int prev_pitch = current_pitch_;
        const bool was_crouched = is_crouched_;
        const bool was_proned = is_proned_;

        interfaces::menu->open();
        interfaces::menu->close();

        interfaces::console->execute("disconnect");
        if (!util::await([] { return window::is_playing_transition_movie(); }, 1min)) {
            throw std::exception("Failed to disconnect.");
        }
        util::await([] {return interfaces::main_menu->is_open(); }, 30s);

        interfaces::main_menu->start();
        interfaces::mode_select->join_game();

        const auto server = network::get_server(settings::last_session_3.get());
        interfaces::server_select->join_server(server->name);

        reset_state();
        set_pitch(prev_pitch);
        if (was_crouched) { crouch(); }
        if (was_proned) { prone(); }
    }

    void LocalPlayer::jump()
    {
        stand_up();
        window::press(settings::jump);

        last_jumped_ = std::chrono::system_clock::now();
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

    bool LocalPlayer::can_ride(const entities::DinoEntity&) const
    {
        return interfaces::hud->can_ride();
    }

    bool LocalPlayer::can_sit_down() const
    {
        return interfaces::hud->can_sit_down();
    }

    void LocalPlayer::access(const BaseEntity& entity, const std::chrono::seconds timeout)
    {
        if (entity.get_inventory()->is_open()) { return; }
        const auto start = std::chrono::system_clock::now();
        do {
            window::press(settings::access_inventory, true);
            if (util::timedout(start, timeout)) { throw EntityNotAccessed(&entity); }
        } while (!util::await([&entity]() { return entity.get_inventory()->is_open(); },
                              10s));

        entity.get_inventory()->receive_remote_inventory(30s);
    }

    void LocalPlayer::access(const structures::Container& container,
                             const std::chrono::seconds timeout)
    {
        // Accessing the inventory is the same as accessing the interface of
        // any interactable structure such as teleporters, beds etc.
        // just that we have to wait to receive the remote inventory afterward.
        access(static_cast<const structures::Interactable&>(container), timeout);
        container.get_inventory()->receive_remote_inventory(30s);
    }

    void LocalPlayer::access(const structures::SimpleBed& bed, const AccessFlags flags)
    {
        static structures::Container bag("Item Cache", 0);

        if (bed.get_interface()->is_open()) { return; }
        const bool special_access_set = (flags & AccessFlags_AccessAboveOrBelow) ==
                                        AccessFlags_AccessAboveOrBelow;
        crouch();
        for (int attempt = 0; attempt < 3; attempt++) {
            if (!special_access_set) { handle_access_direction(flags); }

            // If a bag is seen, give it a few seconds to disappear.
            if (!util::await([this]() -> bool { return !can_access(bag); }, 3s)) {
                access(bag);
                // Check health level to ensure its an item cache.
                if (bag.get_info()->get_health_level() == 0.f) {
                    bag.get_inventory()->popcorn_all();
                } else { reset_pitch(); }
                bag.get_inventory()->close();
                core::sleep_for(1s);
                continue;
            }

            if (special_access_set) { set_pitch(90); }
            if (util::await(interfaces::hud::can_fast_travel, 1s)) {
                break;
            }
            if (special_access_set) {
                set_pitch(-90);
                if (util::await(interfaces::hud::can_fast_travel, 1s)) { break; }
            }

            // Still unable to see the bed, either missing or not yet loaded.
            if (flags & AccessFlags_InstantFail) {
                throw structures::StructureNotFoundError(&bed);
            }

            // TODO: Implement the action wheel as 2nd indicator we are unable to access it
            if (!util::await(interfaces::hud::can_fast_travel, 5s) && attempt != 2) {
                reset_pitch();
            }
        }
        // At this point all the specific work for beds is done, can let the
        // general access method take over the rest.
        access(bed);
    }

    void LocalPlayer::lay_on(const structures::SimpleBed&, const AccessFlags flags)
    {
        handle_access_direction(flags);
        core::sleep_for(1s);

        controls::down(settings::use);
        core::sleep_for(2s);

        const auto location = window::locate_template(window::Rect{683, 253, 543, 556},
                                                      resources::wheel_icon::lay_on);
        if (!location.has_value()) {
            throw std::exception("Failed to locate lay_on position");
        }

        const auto pos = location->get_random_location(3);
        window::set_mouse_pos(pos.x + 683, pos.y + 253);

        core::sleep_for(1s);
        controls::release(settings::use);
    }

    void LocalPlayer::access(const structures::Interactable& structure, const std::chrono::seconds timeout)
    {
        if (structure.get_interface()->is_open()) { return; }
        bool has_reconnected = false;
        auto start = std::chrono::system_clock::now();

        do {
            window::press(structure.get_interact_key(), true);
            if (util::timedout(start, timeout)) {
                // before we throw the error, lets try to reconnect and restore our
                // state in order to handle the render bug
                if (!has_reconnected) {
                    reconnect();
                    start = std::chrono::system_clock::now();
                    has_reconnected = true;
                } else {
                    throw structures::StructureNotOpenedError(&structure);
                }
            }
        } while (!util::await([&structure] {
            return structure.get_interface()->is_open();
        }, 10s));
    }

    void LocalPlayer::mount(DinoEntity& entity)
    {
        const auto start = std::chrono::system_clock::now();
        interfaces::hud->toggle_extended(true);
        core::sleep_for(200ms);

        if (!entity.is_mounted()) {
            do {
                if (util::timedout(start, 1min)) { throw EntityNotMounted(&entity); }
                if (entity.get_inventory()->is_open()) {
                    entity.get_inventory()->close();
                }
                window::press(settings::use);
            } while (!util::await([&entity]() -> bool {
                return entity.is_mounted();
            }, 10s));
        }
        interfaces::hud->toggle_extended(false);
        is_riding_mount_ = true;
    }

    void LocalPlayer::dismount(DinoEntity& entity)
    {
        interfaces::hud->toggle_extended(true);
        core::sleep_for(std::chrono::milliseconds(200));

        if (entity.is_mounted()) {
            do { window::press(settings::use); } while (!util::await(
                [&entity]() -> bool { return !entity.is_mounted(); },
                std::chrono::seconds(5)));
        }
        interfaces::hud->toggle_extended(false);
        is_riding_mount_ = false;
    }

    void LocalPlayer::fast_travel_to(const structures::SimpleBed& bed,
                                     const AccessFlags access_flags,
                                     const TravelFlags travel_flags)
    {
        try { access(bed, access_flags); } catch (const structures::structure_error& e) {
            throw FastTravelFailedError(bed.get_name(), e.what());
        }

        bed.get_interface()->go_to(bed.get_name(),
                                   travel_flags & TravelFlags_WaitForBeds);

        // always wait for the animation to start, dont wait for it to end if
        // the no travel animation flag is set.
        pass_travel_screen(true, !(travel_flags & TravelFlags_NoTravelAnimation));
        reset_view_angles();
        is_crouched_ = false;
        is_proned_ = false;
    }

    void LocalPlayer::teleport_to(const structures::Teleporter& dst,
                                  const TeleportFlags flags)
    {
        static structures::Teleporter generic_teleporter("GENERIC SRC TELEPORTER");
        const auto start = std::chrono::system_clock::now();

        // While riding a mount, the only way we can teleport is the default option.
        if (is_riding_mount_ && !(flags & TeleportFlags_UseDefaultOption)) {
            throw std::exception("Cannot use non default teleport while mounted.");
        }

        if (flags & TeleportFlags_UseDefaultOption) {
            while (is_riding_mount_ && !interfaces::hud->can_default_teleport()) {
                go_back(100ms);
                util::await([] { return interfaces::hud->can_default_teleport(); }, 5s);
                if (util::timedout(start, 30s)) {
                    throw TeleportFailedError(dst.get_name(), "No default destination.");
                }
            }

            do { window::press(settings::reload); } while (!util::await(
                [] { return !interfaces::hud->can_default_teleport(); }, 5s));
        } else {
            set_pitch(90);
            access(generic_teleporter);
            generic_teleporter.get_interface()->go_to(dst.get_name());
            util::await([] { return !interfaces::hud->can_default_teleport(); }, 5s);
        }

        // If the unsafe load flag is set, skip the step of passing the teleportation
        // and just assume that we did.
        if (flags & TeleportFlags_UnsafeLoad) { return; }
        if (!pass_teleport_screen()) {
            throw TeleportFailedError(dst.get_name(), "Did not arrive at destination.");
        }
    }

    void LocalPlayer::get_off_bed()
    {
        window::press(settings::use);
        core::sleep_for(std::chrono::seconds(3));
        reset_view_angles();
    }

    bool LocalPlayer::pass_travel_screen(const bool in, const bool out)
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
        return true;
    }

    bool LocalPlayer::pass_teleport_screen(const bool access_flag)
    {
        const auto start = std::chrono::system_clock::now();

        while (!interfaces::hud->can_default_teleport()) {
            // for long distance teleports we still enter a white screen,
            // so we can simply reuse our bed logic
            if (is_in_travel_screen()) { return pass_travel_screen(false); }
            if (util::timedout(start, 30s)) { return false; }
            if (access_flag && can_access_inventory()) { return true; }

            if (is_riding_mount_ && util::timedout(start, 2s)) {
                go_forward(100ms);
                util::await([] { return interfaces::hud->can_default_teleport(); }, 5s);
                if (util::timedout(start, 30s)) { return false; }
            }
        }
        // See whether the default teleport popup lasts for more than 500ms
        // if it doesnt its a glitched popup that appears when the teleport has
        // happened. Restart the procedure in that case
        if (util::await([] { return !interfaces::hud->can_default_teleport(); }, 500ms)) {
            return pass_teleport_screen();
        }
        return true;
    }

    void LocalPlayer::handle_access_direction(const AccessFlags flags)
    {
        if (flags & AccessFlags_AccessBelow) { set_pitch(90); } else if (
            flags & AccessFlags_AccessAbove) { set_pitch(-90); }
        if (flags & AccessFlags_AccessLeft) { set_yaw(-90); } else if (
            flags & AccessFlags_AccessRight) { set_yaw(90); }
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

    void LocalPlayer::turn_right(const int degrees, const std::chrono::milliseconds delay)
    {
        controls::turn_degrees_lr(degrees, 0);
        current_yaw_ += degrees;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_left(const int degrees, const std::chrono::milliseconds delay)
    {
        controls::turn_degrees_lr(-degrees, 0);
        current_yaw_ -= degrees;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_down(const int degrees, const std::chrono::milliseconds delay)
    {
        const int allowed = std::min(90 - current_pitch_, degrees);
        controls::turn_degrees_ud(0, allowed);
        current_pitch_ += allowed;
        core::sleep_for(delay);
    }

    void LocalPlayer::turn_up(const int degrees, const std::chrono::milliseconds delay)
    {
        const int allowed = std::min(90 + current_pitch_, degrees);
        controls::turn_degrees_ud(0, -allowed);
        current_pitch_ -= allowed;
        core::sleep_for(delay);
    }

    void LocalPlayer::reset_state()
    {
        reset_view_angles();
        is_crouched_ = false;
        is_proned_ = false;
        is_riding_mount_ = false;
    }




}
