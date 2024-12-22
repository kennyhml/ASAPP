#pragma once
#include <array>
#include <string>
#include "iinterface.h"
#include "components/button.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    struct DestinationNotFound : std::exception
    {
        explicit DestinationNotFound(const std::string& t_dst_name);

        [[nodiscard]] char const* what() const override { return info_.c_str(); };

    private:
        std::string info_;
    };

    struct DestinationNotReady : std::exception
    {
        explicit DestinationNotReady(const std::string& t_dst_name);

        [[nodiscard]] char const* what() const override { return info_.c_str(); };

    private:
        std::string info_;
    };

    class BaseTravelMap : public IInterface
    {
    public:
        struct DestinationButton final : components::Button
        {
        public:
            DestinationButton(const int x, const int y) : Button(x, y, 459, 55, 10) {}

            /**
             * @brief Checks whether this destination is ready.
             */
            [[nodiscard]] bool is_ready() const;

            /**
             * @brief Checks whether this destination is on cooldown.
             */
            [[nodiscard]] bool is_on_cooldown() const;

            /**
             * @brief Checks whether this destination is currently selected.
             */
            [[nodiscard]] bool is_selected() const;

            /**
             * @brief Selects this destination as target for our travel.
             */
            void select();

        private:
            static constexpr window::Color hovered_selected_color{83, 39, 1};
            static constexpr window::Color selected_color{128, 64, 2};

            static constexpr window::Color text_selected_color{255, 255, 255};
            static constexpr window::Color text_color{133, 226, 243};
            static constexpr window::Color text_cooldown_color{70, 120, 138};
        };

    public:
        BaseTravelMap();

        /**
         * @brief Checks whether the travel map is open.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Checks whether the travel button is available.
         *
         * @remark The travel button is on cooldown for ~3 seconds when opened.
         */
        [[nodiscard]] virtual bool can_confirm_travel() const;

        /**
         * @brief Searches the given destination and travels to the first suggested
         * target that is not on cooldown.
         *
         * @param destination The name of the travel traget.
         * @param wait_ready If true, waits for the target to be ready.
         *
         * @throws DestinationNotFound when the searched target doesnt exist.
         */
        virtual void go_to(const std::string& destination, bool wait_ready) = 0;

        /**
         * @brief Gets the available destination buttons.
         */
        [[nodiscard]] virtual std::vector<DestinationButton> get_destinations() const;

        /**
         * @brief Gets the first ready destination, if any exist.
         *
         * @param name The name of the destination we are trying to go to.
         * @param wait_ready Wait for any destination to come off cooldown if any exist.
         *
         * @throws DestinationNotFound If no destinations exist
         * @throws DestinationNotReady If wait_ready is false and all are on cooldown
         */
        [[nodiscard]] DestinationButton get_ready_destination(
            const std::string& name, bool wait_ready) const;

    protected:
        std::array<window::Rect, 12> destination_slots_;
        window::Rect day_time_{1570, 77, 272, 54};
        components::Button confirm_button{1461, 931, 367, 60, 10};
    };
}
