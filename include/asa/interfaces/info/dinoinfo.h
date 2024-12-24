#pragma once
#include "baseentityinfo.h"

namespace asa
{
    class dino_info : public base_entity_info
    {
    public:
        enum slot : uint32_t
        {
            SADDLE = 0,
            COSTUME = 3,
        };

        bool is_open() const override;

        bool has_saddle_equipped() const;

    protected:
        button them_{1045, 124, 118, 49};
    };
}
