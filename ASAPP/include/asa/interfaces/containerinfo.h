#pragma once
#include "baseinfo.h"

namespace asa::interfaces
{
    class ContainerInfo : public BaseInfo
    {
    private:
        window::Rect health_bar_{768, 512, 382, 6};
        window::Rect slots_bar_{768, 548, 382, 6};

    public:
        enum Stat
        {
            HEALTH,
            STORAGE,
            WEIGHT
        };

        bool is_open() const override { return true; }

        int get_max_health() override { return 0; }
        int get_current_health() override { return 0; }
        float get_health_level() override;

        virtual int get_max_slots() { return 0; };
        virtual int get_current_slots() { return 0; }
        virtual float get_fill_level();

        int get_max_weight() override;
        int get_current_weight() override;

        std::string get_owner() override { return ""; }
        std::string get_name() override { return ""; }
    };
}
