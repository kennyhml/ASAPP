#pragma once
#include "baseentity.h"
#include "dinoentity.h"

#include "asa/structures/structures.h"
#include "asa/interfaces/inventories/localinventory.h"

enum AccessFlags_ : uint32_t
{
    AccessFlags_None = 0,
    AccessFlags_InstantFail = 1 << 1, // Throw if the bed is not instantly found.
    AccessFlags_AccessAbove = 1 << 2, // Access the bed above.
    AccessFlags_AccessBelow = 1 << 3, // Access the bed below.
    AccessFlags_AccessLeft = 1 << 4,  // Access the bed to the left.
    AccessFlags_AccessRight = 1 << 5, // Access the bed to the right.

    AccessFlags_AccessAboveOrBelow = AccessFlags_AccessAbove | AccessFlags_AccessBelow,
    AccessFlags_Default = AccessFlags_AccessBelow,
};

enum TravelFlags_ : uint32_t
{
    TravelFlags_None = 0,
    TravelFlags_WaitForBeds = 1 << 1,       // Throw an exception when no beds are ready.
    TravelFlags_NoTravelAnimation = 1 << 2, // Return when the travel animation starts.
};

enum TeleportFlags_ : uint32_t
{
    TeleportFlags_None = 0,
    TeleportFlags_UseDefaultOption = 1 << 1, // Assume the destination is the default
    TeleportFlags_UnsafeLoad = 1 << 2,       // Assume an instant teleport, lag unsafe.
};

using access_flags_t = int32_t;
using travel_flags_t = int32_t;
using teleport_flags_t = int32_t;

namespace asa
{
    /**
     * @brief Represents the local player of the game and serves as a controller.
     *
     * Provides functionality to move the player and it's view angles, mount or access
     * entities and containers as well as most other interactions that involve the
     * local player directly.
     *
     * The local player instance may be obtained through @link get_local_player.
     */
    class local_player final : public base_entity
    {
    public:
        /**
         * @brief Gets the local players inventory component.
         *
         * @returns The @link local_inventory component of the player.
         */
        [[nodiscard]] local_inventory* get_inventory() const override;

        /**
         * @brief Checks whether the local player is currently alive.
         *
         * @returns True if the player is alive, false otherwise.
         */
        [[nodiscard]] bool is_alive() const;

        /**
         * @brief Checks whether the local player is out of water.
         *
         * @returns True if the player is out of water, false otherwise.
         */
        [[nodiscard]] bool is_out_of_water() const;

        /**
         * @brief Checks whether the local player is out of food.
         *
         * @returns True if the player is out of food, false otherwise.
         */
        [[nodiscard]] bool is_out_of_food() const;

        /**
         * @brief Checks whether the local player is broken bones.
         *
         * @returns True if the player is broken bones, false otherwise.
         */
        [[nodiscard]] bool is_broken_bones() const;

        /**
         * @brief Checks whether the local player is out of overweight.
         *
         * @returns True if the player is overweight, false otherwise.
         */
        [[nodiscard]] bool is_overweight() const;

        /**
         * @brief Checks whether a certain item was received.
         *
         * @returns True if the player has received the item, false otherwise.
         */
        [[nodiscard]] bool received_item(item&) const;

        /**
         * @brief Checks whether a certain item was deposited.
         *
         * @return True if the player has deposited the item, false otherwise.
         */
        [[nodiscard]] bool deposited_item(item&) const;

        /**
         * @brief Checks whether the player is currently in a travel screen.
         *
         * @return True if the player is in a travel screen, false otherwise.
         */
        [[nodiscard]] bool is_in_travel_screen() const;

        /**
         * @brief Checks whether the player is currently riding a mount.
         *
         * @return True if the player is riding a mount, false otherwise.
         */
        [[nodiscard]] bool is_riding_mount() const;

        /**
         * @brief Checks whether the player can currently access a bed.
         *
         * @return True if a bed is accessible, false otherwise.
         */
        [[nodiscard]] bool can_access_bed() const;

        /**
         * @brief Checks whether the player can currently access an inventory.
         *
         * @return True if an inventory is accessible, false otherwise.
         */
        [[nodiscard]] bool can_access_inventory() const;

        /**
         * @brief Checks whether the player has a default destination available.
         *
         * @return True if a default teleporter destination is available, false otherwise.
         */
        [[nodiscard]] bool can_use_default_teleport() const;

        /**
         * @brief Deposits the given item into a dedicated storage.
         *
         * @param amount_out If provided, the amount of the item that was deposited.
         *
         * @return True if any items were deposited, false otherwise.
         *
         * @throws deposit_failed If the deposit was not successful.
         */
        bool deposit_into_dedi(item&, int* amount_out);

        /**
         * @brief Attempts to turn to the waypoint with the provided color.
         *
         * @param color The color of the waypoint icon.
         * @param variance The variance to match the color with.
         *
         * @return Whether a waypoint was found and targetted.
         */
        bool turn_to_waypoint(const cv::Vec3b& color, float variance);

        /**
         * @brief Checks if the structure can be accessed.
         *
         * TODO: If no "access inventory" is available, optionally check action wheel
         *
         * @return True if the structure is accessible, false otherwise.
         */
        [[nodiscard]] bool can_access(const base_structure&) const;

        /**
         * @brief Checks if the entity can be accessed.
         *
         * TODO: If no "access inventory" or "ride" is available, optionally check action wheel
         *
         * @return True if the entity is accessible, false otherwise.
         */
        [[nodiscard]] bool can_access(const dino_entity&) const;

        /**
         * @brief Checks if the entity can be ridden.
         *
         * TODO: If no "ride" is available, optionally check action wheel
         *
         * @return True if the entity is rideable, false otherwise.
         */
        [[nodiscard]] bool can_ride(const dino_entity&) const;

        /**
         * @brief Checks if the player is currently able to sit down.
         *
         * @return True if its possible to sit down, false otherwise.
         */
        [[nodiscard]] bool can_sit_down() const;

        void access(const base_entity&, std::chrono::seconds max = 30s);

        void access(const container&, std::chrono::seconds max = 30s);

        void access(const interactable&, std::chrono::seconds max = 30s);

        void access(const simple_bed&, access_flags_t);

        void lay_on(const simple_bed&, access_flags_t);

        void pick_up_one() const;

        void pick_up_all() const;

        /**
         * @brief Mounts the local player onto a given (rideable) entity.
         *
         * @throws EntityNotMounted If the dino could not be mounted within 60s.
         */
        void mount(dino_entity& entity);

        /**
         * @brief Dismounts the local player from a given (rideable) entity.
         */
        void dismount(dino_entity& entity);

        /**
         * @brief Teleports to a target destination.
         *
         * @param dst The teleporter that serves as the destination.
         * @param flags Flags to control the teleport process.
         */
        void teleport_to(const teleporter& dst,
                         teleport_flags_t flags = TeleportFlags_None);

        /**
         * @brief Fast travels to a target destination.
         *
         * @param dst The bed that serves as the destination.
         * @param access_flags Flags to control the process of accessing the bed.
         * @param travel_flags Flags to control the travelling process.
         */
        void fast_travel_to(const simple_bed& dst,
                            access_flags_t access_flags = AccessFlags_Default,
                            travel_flags_t travel_flags = TravelFlags_None);

        void get_off_bed();

        void suicide();

        /**
         * @brief Override to ensure we leave any crouched / proned states first.
         */
        void jump() override;

        /**
         * @brief Makes the local player enter a crouched state.
         */
        void crouch();

        /**
         * @brief Makes the local player enter a proned state.
         */
        void prone();

        /**
         * @brief Makes the local player enter the default state.
         */
        void stand_up();

        /**
         * @brief Handles the access direction of an access flag bitfield.
         */
        void handle_access_direction(access_flags_t);

        /**
         * @brief Gets the current yaw angle of the player's view.
         *
         * The yaw angle represents the horizontal orientation of the player's view
         * in degrees, used to determine the direction the player is facing.
         *
         * @remark The  valid range of the player's yaw is between -180 and 180.
         *
         * @return The current yaw angle in degrees.
         */
        [[nodiscard]] int get_yaw() const { return current_yaw_; }

        /**
         * @brief Gets the current pitch angle of the player's view.
         *
         * The pitch angle represents the vertical orientation of the player's view
         * in degrees, used to determine the upward or downward tilt of itsviewpoint.
         *
         * @remark The valid range of the player's pitch is between -90 and 90 degrees.
         *
         * @return The current pitch angle in degrees.
         */
        [[nodiscard]] int get_pitch() const { return current_pitch_; }

        /**
         * @brief Sets the yaw angle of the players's view.
         *
         * @param yaw The new yaw angle, between -180 (left) and 180 (right)
         *
         * @remark Does not only modify the internal yaw state but also attempts
         * to sync in the game.
         */
        void set_yaw(int yaw);

        /**
         * @brief Sets the pitch angle of the players's view.
         *
         * @param pitch The new pitch angle, between -90 (up) and 90 (down)
         *
         * @remark Does not only modify the internal pitch state but also attempts
         * to sync in the game.
         */
        void set_pitch(int pitch);

        /**
         * @brief Resets the internal yaw view angle state of the player.
         *
         * @note Certain actions in the game (fast travelling, laying on the bed, spawning...)
         * will reset the yaw and pitch of the player.
         */
        void reset_yaw() { current_yaw_ = 0; }

        /**
         * @brief Resets the internal pitch view angle state of the player.
         *
         * @note Certain actions in the game (fast travelling, laying on the bed, spawning...)
         * will reset the yaw and pitch of the player.
         */
        void reset_pitch() { current_pitch_ = 0; }

        /**
         * @brief Resets the internal view angles (yaw and pitch) of the player.
         *
         * @remarks Certain actions in the game (fast travelling, laying on the bed, spawning...)
         * will reset the yaw and pitch of the player.
         */
        void reset_view_angles()
        {
            reset_yaw();
            reset_pitch();
        }

        /**
         * @brief Turns the view of this entity to the right by the given amount.
         *
         * @param degrees The amount of degrees to turn.
         * @param delay The amount of time to wait after turning.
         */
        void turn_right(int degrees = 90, std::chrono::milliseconds delay = 100ms);

        /**
         * @brief Turns the view of this entity to the left by the given amount.
         *
         * @param degrees The amount of degrees to turn.
         * @param delay The amount of time to wait after turning.
         */
        void turn_left(int degrees = 90, std::chrono::milliseconds delay = 100ms);

        /**
         * @brief Turns the view of this entity to the up by the given amount.
         *
         * @param degrees The amount of degrees to turn.
         * @param delay The amount of time to wait after turning.
         *
         * @remark The pitch (up / down) reaches its limit at 90 or -90.
         */
        void turn_up(int degrees = 90, std::chrono::milliseconds delay = 100ms);

        /**
         * @brief Turns the view of this entity to the down by the given amount.
         *
         * @param degrees The amount of degrees to turn.
         * @param delay The amount of time to wait after turning.
         *
         * @remark The pitch (up / down) reaches its limit at 90 or -90.
         */
        void turn_down(int degrees = 90, std::chrono::milliseconds delay = 100ms);

        /**
         * @brief Resets the states of the local player including:
         * - Crouched state
         * - Proned state
         * - Mounted state
         * - Yaw angle
         * - Pitch angle
         */
        void reset_state();

    private:
        explicit local_player()
            : base_entity("You", std::make_unique<local_inventory>()) {}

        bool pass_travel_screen(bool in = true, bool out = true);

        bool pass_teleport_screen(bool access_flag = false);

        friend std::shared_ptr<local_player> get_local_player();

        int current_yaw_ = 0; // degrees of our view left and right, between -180 and 180
        int current_pitch_ = 0; // degrees of our view bottom to top, between -90 and 90

        bool is_riding_mount_ = false;
        bool is_crouched_ = false;
        bool is_proned_ = false;

        static std::unique_ptr<local_player> instance;
        static std::once_flag init_flag;

        std::chrono::system_clock::time_point last_jumped_;
    };

    [[nodiscard]] std::shared_ptr<local_player> get_local_player();
}
