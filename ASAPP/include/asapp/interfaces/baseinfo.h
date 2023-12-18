#pragma once
#include "iinterface.h"
#include "asapp/game/window.h"

namespace asa::interfaces
{
    class BaseInfo : public IInterface
    {
    protected:
        window::Rect area{747, 109, 429, 806};

    public:
        enum Stat
        {
            HEALTH,
            WEIGHT,
        };

        virtual int get_max_health() = 0;
        virtual int get_current_health() = 0;

        virtual int get_max_weight() = 0;
        virtual int get_current_weight() = 0;

        virtual std::string get_owner() = 0;
        virtual std::string get_name() = 0;
    };
}
