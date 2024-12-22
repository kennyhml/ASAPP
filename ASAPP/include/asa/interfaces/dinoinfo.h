#pragma once
#include "baseentityinfo.h"

namespace asa::interfaces
{
    class DinoInfo : public BaseEntityInfo
    {
    public:
        enum Slot : uint32_t
        {
            SADDLE = 0,
            COSTUME = 3,
        };

        bool is_open() const override;

        bool has_saddle_equipped() const;

    protected:
        components::Button them_{1045, 124, 118, 49};
    };
}
