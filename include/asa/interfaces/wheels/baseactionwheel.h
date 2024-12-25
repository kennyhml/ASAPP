#pragma once
#include "asa/interfaces/asainterface.h"
#include "asa/game/window.h"

namespace asa
{
    class base_action_wheel : public asainterface
    {

    public:
        /**
         * @brief Checks if the action wheel is open.
         *
         * @return True if the action wheel is open, false otherwise.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * TODO: Refactor into a more generic variant
         */
        void select_lay_on() const;

    private:
        cv::Rect name_area{815, 461, 294, 160};
        cv::Rect area{683, 253, 543, 556};
    };
}
