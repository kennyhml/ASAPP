#pragma once

namespace asa
{
    class asainterface
    {
    public:
        virtual ~asainterface() = default;

        [[nodiscard]] virtual bool is_open() const = 0;
    };
}
