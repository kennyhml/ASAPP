#pragma once
#include "iinterface.h"
#include "asa/game/window.h"

namespace asa::interfaces
{
    class BaseInfo : public IInterface
    {
    protected:
        window::Rect area{747, 109, 429, 806};

    public:
        enum struct Stat : uint32_t
        {
            HEALTH,
            WEIGHT,
        };

        bool is_open() const override { return true; };

        virtual int get_max_health() { return 0; }
        virtual int get_current_health() { return 0; }
        virtual float get_health_level() { return 0; }

        virtual int get_max_weight() { return 0; }
        virtual int get_current_weight() { return 0; }

        virtual std::string get_owner() { return ""; }
        virtual std::string get_name() { return ""; }
    };
}
