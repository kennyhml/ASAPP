#pragma once
#include "baseentity.h"
#include "asapp/interfaces/localinventory.h"
#include "asapp/structures/basestructure.h"
#include "asapp/structures/container.h"
#include "asapp/structures/simplebed.h"
#include "asapp/structures/teleporter.h"
#include "dinoent.h"

namespace asa::entities
{
    class LocalPlayer : public BaseEntity
    {
    public:
        explicit LocalPlayer() : BaseEntity(
            "You", std::make_unique<interfaces::LocalInventory>()) {}

        /**
         * @brief Gets the local player inventory component.
         */
        [[nodiscard]] interfaces::LocalInventory* get_inventory() const override;

        /**
         * @brief Checks whether the local player is currently alive.
         */
        [[nodiscard]] bool is_alive() const;

        /**
         * @brief Checks whether the local player is out of water.
         */
        [[nodiscard]] bool is_out_of_water() const;

        /**
         * @brief Checks whether the local player is out of food.
         */
        [[nodiscard]] bool is_out_of_food() const;

        /**
         * @brief Checks whether the local player is broken bones.
         */
        [[nodiscard]] bool is_broken_bones() const;

        /**
         * @brief Checks whether the local player is out of overweight.
         */
        [[nodiscard]] bool is_overweight() const;

        [[nodiscard]] bool received_item(items::Item&) const;
        [[nodiscard]] bool deposited_item(items::Item&) const;
        [[nodiscard]] bool is_in_spawn_animation() const;
        [[nodiscard]] bool is_in_travel_screen() const;
        [[nodiscard]] bool can_access_bed() const;
        [[nodiscard]] bool can_access_inventory() const;
        [[nodiscard]] bool can_use_default_teleport() const;

        bool deposit_into_dedi(items::Item&, int* amount_out);
        bool withdraw_from_dedi(items::Item&, int* amount_out);
        bool get_amount_added(items::Item&, int& amount_out);
        bool get_amount_removed(items::Item&, int& amount_out);

        [[nodiscard]] bool can_access(const structures::BaseStructure&) const;
        [[nodiscard]] bool can_access(const entities::BaseEntity&) const;

        void access(const BaseEntity&) const;
        void access(const structures::Container&) const;
        void access(const structures::InteractableStructure&) const;

        void mount(const DinoEnt&) const;

        void fast_travel_to(const structures::SimpleBed&);
        void teleport_to(const structures::Teleporter&, bool is_default = false);
        void get_off_bed();
        void suicide();

        void walk_forward(std::chrono::milliseconds duration);
        void walk_left(std::chrono::milliseconds duration);
        void walk_right(std::chrono::milliseconds duration);
        void walk_back(std::chrono::milliseconds duration);

        void equip(items::Item* item, interfaces::PlayerInfo::Slot target_slot);
        void unequip(interfaces::PlayerInfo::Slot target_slot);

        void look_fully_down();
        void look_fully_up();

        void jump();
        void crouch();
        void prone();
        void stand_up();

    private:
        bool is_crouched_ = false;
        bool is_proned_ = false;

        void pass_travel_screen(bool in = true, bool out = true);
        void pass_teleport_screen(bool access_flag = false);

    public:
        using ms = std::chrono::milliseconds;

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

        void turn_right(int by_degrees = 90, ms delay = ms(100));
        void turn_left(int by_degrees = 90, ms delay = ms(100));
        void turn_up(int by_degrees = 90, ms delay = ms(100));
        void turn_down(int by_degrees = 90, ms delay = ms(100));

    private:
        int current_yaw_ = 0; // degrees of our view left and right, between -180 and 180
        int current_pitch_ = 0; // degrees of our view bottom to top, between -90 and 90
    };

    inline std::unique_ptr<LocalPlayer> local_player = std::make_unique<LocalPlayer>();
}
