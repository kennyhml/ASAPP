#pragma once

#include "iinterface.h"
#include "../game/window.h"

namespace asa::interfaces
{
    class ActionWheel : public IInterface
    {
    private:
        window::Rect name_area{815, 461, 294, 160};
        window::Rect area{683, 253, 543, 556};

    public:
        bool is_open() const override;
        void select_lay_on() const;
    };
}
