#pragma once

namespace asa
{
    class interface
    {
    public:
        virtual ~interface() = default;

        [[nodiscard]] virtual bool is_open() const = 0;
    };
}
