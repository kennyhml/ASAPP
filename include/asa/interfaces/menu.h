#pragma once
#include "interface.h"

namespace asa
{
    class menu final : public interface
    {
    public:
        [[nodiscard]] bool is_open() const override;

        void open();

        void close();
    };
}
