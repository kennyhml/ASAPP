#pragma once
#include "../interfaces/actionwheel.h"
#include "../interfaces/baseinventory.h"


namespace asa::entities
{
    class BaseEntity
    {
    public:
        explicit BaseEntity(std::string t_name,
                            std::unique_ptr<interfaces::BaseInventory> t_inventory =
                                nullptr)
            : name_(std::move(t_name)),
              inventory_(t_inventory
                             ? std::move(t_inventory)
                             : std::make_unique<interfaces::BaseInventory>(true)) {}

        /**
         * @brief Gets the name of the entity.
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

        /**
         * @brief Gets the inventory component of the entity.
         */
        [[nodiscard]] virtual interfaces::BaseInventory* get_inventory() const
        {
            return inventory_.get();
        }

        /**
         * @brief Uses the primary attack of the entity (left click) 
         */
        virtual void primary_attack();

        /**
         * @brief Uses the secondary attack of the entity (right click) 
         */
        virtual void secondary_attack();

        /**
         * @brief Goes forward for a specified duration.
         *
         * @param duration How long to go forward for
         */
        void go_forward(const std::chrono::milliseconds duration) { walk("w", duration); }

        /**
         * @brief Goes backwards for a specified duration.
         *
         * @param duration How long to go back for
         */
        void go_back(const std::chrono::milliseconds duration) { walk("s", duration); }

        /**
         * @brief Goes left for a specified duration.
         *
         * @param duration How long to go left for
         */
        void go_left(const std::chrono::milliseconds duration) { walk("a", duration); }

        /**
         * @brief Goes right for a specified duration.
         *
         * @param duration How long to go right for
         */
        void go_right(const std::chrono::milliseconds duration) { walk("d", duration); }

        /**
         * @brief Makes the local player jump.
         *
         * @remark If the local player is currently in a crouched or proned state,
         * it will first stand up, then jump.
         */
        virtual void jump();

    protected:
        std::chrono::system_clock::time_point last_primary_attack_;
        std::chrono::system_clock::time_point last_secondary_attack_;
        std::chrono::system_clock::time_point last_jumped_;
        std::chrono::system_clock::time_point last_moved_;

        std::string name_;
        std::unique_ptr<interfaces::BaseInventory> inventory_;
        interfaces::ActionWheel action_wheel_;

        void walk(const std::string& key, std::chrono::milliseconds duration);
    };
}
