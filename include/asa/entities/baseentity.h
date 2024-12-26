#pragma once
#include "asa/ui/wheels/baseactionwheel.h"
#include "asa/ui/storage/baseinventory.h"

namespace asa
{
    /**
     * @brief Base class for any entity-like actor in ASA.
     *
     * Provides the basic attributes and functionality of any entity.
     * Component getters may be overriden by subclasses to provide concrete implementations.
     */
    class base_entity
    {
    public:
        virtual ~base_entity() = default;

        /**
         * @brief Constructors a base entity with a given name and it's inventory component.
         *
         * @param t_name The name of the entity, may also refer to its kind.
         * @param t_inv An inventory component that belongs to this entity, created if null.
         */
        explicit base_entity(std::string t_name,
                             std::unique_ptr<base_inventory> t_inv = nullptr);

        /**
         * @brief Gets the name of the entity.
         */
        [[nodiscard]] const std::string& get_name() const { return name_; }

        /**
         * @brief Gets the inventory component of the entity.
         *
         * Subclasses may override this to provide a more concrete inventory component.
         */
        [[nodiscard]] virtual base_inventory* get_inventory() const
        {
            return inventory_.get();
        }

        /**
         * @brief Gets the action wheel component of the entity.
         *
         * Subclasses may override this to provide a more concrete wheel component.
         */
        [[nodiscard]] virtual base_action_wheel* get_action_wheel() const
        {
            return action_wheel_.get();
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
         * @brief Makes the entity jump.
         */
        virtual void jump();

    protected:
        std::chrono::system_clock::time_point last_primary_attack_;
        std::chrono::system_clock::time_point last_secondary_attack_;
        std::chrono::system_clock::time_point last_jumped_;
        std::chrono::system_clock::time_point last_moved_;

        std::string name_;
        std::unique_ptr<base_inventory> inventory_;
        std::unique_ptr<base_action_wheel> action_wheel_;

        void walk(const std::string& key, std::chrono::milliseconds duration);
    };
}
