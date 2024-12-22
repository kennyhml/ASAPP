#pragma once

namespace asa
{
    class interface
    {
    public:
        [[nodiscard]] virtual bool is_open() const = 0;
    };
}
