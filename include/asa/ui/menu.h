#pragma once
#include "asainterface.h"

namespace asa
{
    class menu final : public asainterface
    {
    public:
        [[nodiscard]] bool is_open() const override;

        void open();

        void close();
    };
}
