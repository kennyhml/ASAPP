#include "asa/entities/localplayer.h"
#include "asa/entities/exceptions.h"
#include "asa/network/queries.h"
#include "asa/ui/interfaces.h"
#include "asa/game/game.h"
#include "asa/utility.h"
#include "asa/core/logging.h"
#include "asa/core/state.h"

namespace asa
{
    namespace
    {
        int normalize_yaw(int yaw)
        {
            // Normalize to [0, 360]
            yaw = (yaw % 360 + 360) % 360;
            if (yaw > 180) { yaw -= 360; } // Convert to [-180, 180]
            return yaw;
        }
    }


    local_inventory* local_player::get_inventory() const
    {
        return dynamic_cast<local_inventory*>(inventory_.get());
    }

    bool local_player::is_alive() const
    {
        get_hud()->toggle_extended(true);
        const bool result = utility::await([] {
            return get_hud()->is_extended_info_toggled();
        }, 300ms);

        get_hud()->toggle_extended(false);
        return result;
    }

    bool local_player::is_out_of_water() const
    {
        return get_hud()->is_player_out_of_water();
    }

    bool local_player::is_out_of_food() const
    {
        return get_hud()->is_player_out_of_food();
    }

    bool local_player::is_broken_bones() const
    {
        return get_hud()->is_player_broken_bones();
    }

    bool local_player::is_overweight() const
    {
        return get_hud()->is_player_overweight();
    }

    bool local_player::received_item(item& item) const
    {
        return get_hud()->item_added(item, nullptr);
    }

    bool local_player::deposited_item(item& item) const
    {
        return get_hud()->item_removed(item, nullptr);
    }

    bool local_player::is_in_travel_screen() const
    {
        static cv::Rect roi(94, 69, 1751, 883);

        cv::Mat image = screenshot(roi);
        cv::Mat gray;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        cv::Scalar mean, stddev;
        meanStdDev(gray, mean, stddev);
        return mean[0] > 240.f;
    }

    // bool local_player::is_in_connect_screen() const
    // {
    //     static cv::Rect roi(94, 69, 1751, 883);
    //
    //     cv::Mat image = screenshot(roi);
    //     cv::Mat gray;
    //     cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    //
    //     cv::Scalar mean, stddev;
    //     meanStdDev(gray, mean, stddev);
    //     return mean[0] < 5.f;
    // }

    bool local_player::is_riding_mount() const
    {
        return get_hud()->mount_hud_available();
    }

    bool local_player::can_perform(const PlayerInteraction interaction) const
    {
        switch (interaction) {
            case PlayerInteraction_DefaultTeleport:
                return get_hud()->can_default_teleport();
        }

        cv::Mat screen = screenshot();
        const auto narrowed = utility::find_multi_interactable_line(screen);
        if (!narrowed.has_value()) { return false; }

        const cv::Rect roi = {narrowed->x, narrowed->y - 25, narrowed->width, 25};

        switch (interaction) {
            case PlayerInteraction_Teleport:
                return match(embedded::text::teleport_to, screen(roi));
            case PlayerInteraction_AccessBed:
                return match(embedded::text::fast_travel, screen(roi));
            case PlayerInteraction_SitDown:
                return match(embedded::text::sit_on, screen(roi));
            case PlayerInteraction_Deposit:
                return match(embedded::text::deposit, screen(roi));
            case PlayerInteraction_PickUp:
                return match(embedded::text::pick_up, screen(roi));
            case PlayerInteraction_AccessInventory:
                return match(embedded::text::access_inventory, screen(roi));
        }
    }

    bool local_player::deposit_into_dedi(item& item, int* amount_out)
    {
        auto deposited = [&item, amount_out]() -> bool {
            if (amount_out) { return get_hud()->count_items_removed(item, *amount_out); }
            return get_hud()->item_removed(item, nullptr);
        };

        const auto start = std::chrono::system_clock::now();
        do {
            if (utility::timedout(start, 15s)) {
                throw deposit_failed(item.get_name());
            }
            post_press(get_action_mapping("Use"));
        } while (!utility::await(deposited, 5s));
        return true;
    }

    bool local_player::turn_to_waypoint(const cv::Vec3b& color,
                                        const float variance)
    {
        const cv::Mat screen = screenshot();
        const cv::Mat masked = utility::mask(screen, color, variance);

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

        turn_to(rect.x + rect.width / 2, rect.y + rect.height / 2);
        checked_sleep(100ms);
        return true;
    }

    void local_player::suicide()
    {
        const auto start = std::chrono::system_clock::now();
        get_hud()->toggle_extended(true);

        get_inventory()->open();

        // Sometimes the implant doesnt show, in that case we need to re-open the inventory.
        if (get_inventory()->slots[0].is_empty()) {
            get_inventory()->close();
            return suicide();
        }

        checked_sleep(100ms);
        get_inventory()->select_slot(0, true, true);
        checked_sleep(6s);

        do {
            if (utility::timedout(start, 30s)) { throw suicide_failed(); }
            // right click the implant to resolve the glitched implant
            post_press(MouseButton::RIGHT);
            post_press(get_action_mapping("Use"));
            checked_sleep(std::chrono::seconds(3));
        } while (get_hud()->is_extended_info_toggled());
        while (!get_spawn_map()->is_open()) {}

        reset_state();
        get_hud()->toggle_extended(false, true);
    }

    //void local_player::reconnect()
    //{
    //    const int prev_pitch = current_pitch_;
    //    const bool was_crouched = is_crouched_;
    //    const bool was_proned = is_proned_;
    //
    //    menu->open();
    //    menu->close();
    //
    //    console->execute("disconnect");
    //    if (!utility::await([] { return is_playing_transition_movie(); }, 1min)) {
    //        throw std::exception("Failed to disconnect.");
    //    }
    //    utility::await([] { return main_menu->is_open(); }, 30s);
    //
    //    main_menu->start();
    //    mode_select->join_game();
    //
    //    const auto server = network::get_server(settings::last_session_3.get());
    //    server_select->join_server(server->name);
    //
    //    reset_state();
    //    set_pitch(prev_pitch);
    //    if (was_crouched) { crouch(); }
    //    if (was_proned) { prone(); }
    //}

    void local_player::jump()
    {
        stand_up();
        post_press(get_action_mapping("Jump"));

        last_jumped_ = std::chrono::system_clock::now();
    }

    void local_player::crouch()
    {
        if (!is_crouched_) { post_press(get_action_mapping("Crouch")); }
        is_crouched_ = true;
        is_proned_ = false;
    }

    void local_player::prone()
    {
        if (!is_proned_) { post_press(get_action_mapping("Prone")); }
        is_proned_ = true;
        is_crouched_ = false;
    }

    void local_player::stand_up()
    {
        if (is_proned_ || is_crouched_) { post_press(get_action_mapping("Run")); }
        is_proned_ = false;
        is_crouched_ = false;
    }

    void local_player::access(const base_entity& entity, const std::chrono::seconds max)
    {
        // Make sure it's not already open, otherwise we would be closing it.
        if (entity.get_inventory()->is_open()) { return; }

        const auto start = std::chrono::system_clock::now();
        do {
            post_press(get_action_mapping("Access Inventory"));
            if (utility::timedout(start, max)) {
                throw entity_access_failed(&entity);
            }
        } while (!utility::await([&entity]() -> bool {
            return entity.get_inventory()->is_open();
        }, 10s));

        // Wait to receive the remote inventory
        entity.get_inventory()->receive_remote_inventory(30s);
    }

    void local_player::access(const interactable& structure,
                              const std::chrono::seconds max)
    {
        if (structure.get_interface()->is_open()) { return; }
        bool has_reconnected = false;
        auto start = std::chrono::system_clock::now();

        do {
            post_press(structure.get_interact_key());
            if (utility::timedout(start, max)) {
                // before we throw the error, lets try to reconnect and restore our
                // state in order to handle the render bug
                if (!has_reconnected) {
                    // reconnect();
                    start = std::chrono::system_clock::now();
                    has_reconnected = true;
                } else {
                    throw structure_access_failed(structure);
                }
            }
        } while (!utility::await([&structure] {
            return structure.get_interface()->is_open();
        }, 10s));
    }


    void local_player::access(const container& container, const std::chrono::seconds max)
    {
        // Accessing the inventory is the same as accessing the interface of
        // any interactable structure such as teleporters, beds etc.
        // just that we have to wait to receive the remote inventory afterward.
        access(static_cast<const interactable&>(container), max);
        container.get_inventory()->receive_remote_inventory(30s);
    }

    void local_player::access(const simple_bed& bed, const access_flags_t flags)
    {
        static container bag("Item Cache", 0);

        if (bed.get_interface()->is_open()) { return; }
        const bool special_access_set = (flags & AccessFlags_AccessAboveOrBelow) ==
                                        AccessFlags_AccessAboveOrBelow;
        crouch();
        for (int attempt = 0; attempt < 3; attempt++) {
            if (!special_access_set) { handle_access_direction(flags); }

            // If a bag is seen, give it a few seconds to disappear.
            if (!utility::await([this] {
                return !can_perform(PlayerInteraction_AccessInventory);
            }, 3s)) {
                access(bag);
                // Check health level to ensure its an item cache.
                if (bag.get_info()->get_health_level() == 0.f) {
                    bag.get_inventory()->popcorn_all();
                } else { reset_pitch(); }
                bag.get_inventory()->close();
                checked_sleep(1s);
                continue;
            }

            if (special_access_set) { set_pitch(90); }
            if (utility::await([this] {
                return can_perform(PlayerInteraction_FastTravel);
            }, 1s)) { break; }
            if (special_access_set) {
                set_pitch(-90);
                if (utility::await([this] {
                    return can_perform(PlayerInteraction_FastTravel);
                }, 1s)) { break; }
            }

            // Still unable to see the bed, either missing or not yet loaded.
            if (flags & AccessFlags_InstantFail) {
                throw target_not_in_range(bed.get_name());
            }

            // TODO: Implement the action wheel as 2nd indicator we are unable to access it
            if (!utility::await([this] {
                return can_perform(PlayerInteraction_FastTravel);
            }, 5s) && attempt != 2) {
                reset_pitch();
            }
        }
        // At this point all the specific work for beds is done, can let the
        // general access method take over the rest.
        access(bed);
    }

    void local_player::lay_on(const simple_bed&, const access_flags_t flags)
    {
        handle_access_direction(flags);
        checked_sleep(1s);

        post_down(get_action_mapping("Use"));
        checked_sleep(2s);

        const auto location = locate(embedded::wheel_actions::lay_on,
                                             cv::Rect{683, 253, 543, 556});
        if (!location.has_value()) {
            throw std::exception("Failed to locate lay_on position");
        }

        const auto pos = utility::center_of(*location);
        set_mouse_pos({pos.x + 683, pos.y + 253});

        checked_sleep(1s);
        post_up(get_action_mapping("Use"));
    }

    void local_player::mount(dino_entity& entity)
    {
        const auto start = std::chrono::system_clock::now();
        get_hud()->toggle_extended(true);
        checked_sleep(200ms);

        if (!is_riding_mount()) {
            do {
                if (utility::timedout(start, 1min)) {
                    throw entity_mount_failed(&entity);
                }
                if (entity.get_inventory()->is_open()) {
                    entity.get_inventory()->close();
                }
                post_press(get_action_mapping("Use"));
            } while (!utility::await([this] { return is_riding_mount(); }, 10s));
        }
        checked_sleep(200ms);
        get_hud()->toggle_extended(false);
        is_riding_mount_ = true;
    }

    void local_player::dismount(dino_entity& entity)
    {
        get_hud()->toggle_extended(true);
        checked_sleep(std::chrono::milliseconds(200));

        if (is_riding_mount()) {
            do {
                post_press(get_action_mapping("Use"));
            } while (!utility::await([this] { return !is_riding_mount(); }, 5s));
        }
        get_hud()->toggle_extended(false);
        is_riding_mount_ = false;
    }

    void local_player::fast_travel_to(const simple_bed& bed,
                                      const access_flags_t access_flags,
                                      const travel_flags_t travel_flags)
    {
        try {
            access(bed, access_flags);
        } catch (const structure_access_failed& e) {
            throw fast_travel_failed(bed.get_name(), e.what());
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

    void local_player::teleport_to(const teleporter& dst, const teleport_flags_t flags)
    {
        static teleporter generic_teleporter("UNKNOWN SRC TELEPORTER");
        const auto start = std::chrono::system_clock::now();

        // While riding a mount, the only way we can teleport is the default option.
        if (is_riding_mount_ && !(flags & TeleportFlags_UseDefaultOption)) {
            throw std::exception("Cannot use non default teleport while mounted.");
        }

        if (flags & TeleportFlags_UseDefaultOption) {
            while (is_riding_mount_ && !get_hud()->can_default_teleport()) {
                go_back(100ms);
                utility::await([] { return get_hud()->can_default_teleport(); }, 5s);
                if (utility::timedout(start, 30s)) {
                    throw teleport_failed(dst.get_name(), "No default destination.");
                }
            }

            do {
                post_press(get_action_mapping("Reload"));
            } while (!utility::await([] { return !get_hud()->can_default_teleport(); },
                                     5s));
        } else {
            set_pitch(90);
            access(generic_teleporter);
            generic_teleporter.get_interface()->go_to(dst.get_name());
            utility::await([] { return !get_hud()->can_default_teleport(); }, 5s);
        }

        // If the unsafe load flag is set, skip the step of passing the teleportation
        // and just assume that we did.
        if (flags & TeleportFlags_UnsafeLoad) { return; }
        if (!pass_teleport_screen()) {
            throw teleport_failed(dst.get_name(), "Did not arrive at destination.");
        }
    }

    void local_player::get_off_bed()
    {
        post_press(get_action_mapping("Reload"));
        checked_sleep(std::chrono::seconds(3));
        reset_view_angles();
    }

    bool local_player::pass_travel_screen(const bool in, const bool out)
    {
        if (in) {
            if (!utility::await([this]() { return is_in_travel_screen(); },
                                std::chrono::seconds(30))) {}
        }

        if (out) {
            if (!utility::await([this]() { return !is_in_travel_screen(); },
                                std::chrono::seconds(30))) {}
        }

        checked_sleep(std::chrono::seconds(1));
        return true;
    }

    bool local_player::pass_teleport_screen(const bool access_flag)
    {
        const auto start = std::chrono::system_clock::now();

        while (!get_hud()->can_default_teleport()) {
            // for long distance teleports we still enter a white screen,
            // so we can simply reuse our bed logic
            if (is_in_travel_screen()) { return pass_travel_screen(false); }
            if (utility::timedout(start, 30s)) { return false; }

            if (access_flag && can_perform(PlayerInteraction_AccessInventory)) {
                return true;
            }

            if (is_riding_mount_ && utility::timedout(start, 2s)) {
                go_forward(100ms);
                utility::await([] { return get_hud()->can_default_teleport(); }, 5s);
                if (utility::timedout(start, 30s)) { return false; }
            }
        }
        // See whether the default teleport popup lasts for more than 500ms
        // if it doesnt its a glitched popup that appears when the teleport has
        // happened. Restart the procedure in that case
        if (utility::await([] { return !get_hud()->can_default_teleport(); }, 500ms)) {
            return pass_teleport_screen();
        }
        return true;
    }

    void local_player::handle_access_direction(const access_flags_t flags)
    {
        if (flags & AccessFlags_AccessBelow) { set_pitch(90); } else if (
            flags & AccessFlags_AccessAbove) { set_pitch(-90); }
        if (flags & AccessFlags_AccessLeft) { set_yaw(-90); } else if (
            flags & AccessFlags_AccessRight) { set_yaw(90); }
    }

    void local_player::set_yaw(const int yaw)
    {
        const int diff = ((yaw - current_yaw_) + 180) % 360 - 180;
        diff < 0 ? turn_left(-diff) : turn_right(diff);
        current_yaw_ = yaw;
    }

    void local_player::set_pitch(const int pitch)
    {
        const int diff = pitch + current_pitch_;
        diff < 0 ? turn_up(-diff) : turn_down(diff);
        current_pitch_ = pitch;
    }

    void local_player::turn_right(const int degrees,
                                  const std::chrono::milliseconds delay)
    {
        turn(degrees, 0);
        current_yaw_ = normalize_yaw(current_yaw_ + degrees);
        checked_sleep(delay);
        get_logger()->debug("Player turned right {} to {}.", degrees, current_yaw_);
    }

    void local_player::turn_left(const int degrees, const std::chrono::milliseconds delay)
    {
        turn(degrees, 0);
        current_yaw_ = normalize_yaw(current_yaw_ - degrees);
        checked_sleep(delay);
        get_logger()->debug("Player turned left {} to {}.", degrees, current_yaw_);
    }

    void local_player::turn_down(const int degrees, const std::chrono::milliseconds delay)
    {
        // Get the max amount of pitch that we can still turn down, otherwise we may
        // "overturn" which causes the camera to flip up instead.
        // For example, if our pitch is at -40 but the degrees to turn down is 70,
        // then the max we can do is -90 + 40 = -50, which is bigger than -70.
        // We need to think of bigger here instead of smaller because its always negative!
        const int allowed = std::min(abs(PLAYER_PITCH_MIN - current_pitch_), degrees);
        turn(0, allowed);
        current_pitch_ -= allowed;
        checked_sleep(delay);
    }

    void local_player::turn_up(const int degrees, const std::chrono::milliseconds delay)
    {
        // Get the max amount of pitch that we can still turn up, otherwise we may
        // "overturn" which causes the camera to flip down instead.
        // For example, if our pitch is at 70 but the degrees to turn up is 70,
        // then the max we can do is 87 - 70 = 17, which is smaller than 70.
        const int allowed = std::min(PLAYER_PITCH_MAX - current_pitch_, degrees);
        turn(0, -allowed);
        current_pitch_ += allowed;
        checked_sleep(delay);
    }

    void local_player::reset_state()
    {
        reset_view_angles();
        is_crouched_ = false;
        is_proned_ = false;
        is_riding_mount_ = false;
    }

    local_player* get_local_player()
    {
        static auto instance = new local_player();
        return instance;
    }
}
