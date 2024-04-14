#pragma once
#include <memory>

#include "iinterface.h"

namespace asa::interfaces
{
    class Menu final : public IInterface
    {
    public:
        [[nodiscard]] bool is_open() const override;

        void open();

        void close();
    };

    inline auto menu = std::make_unique<Menu>();
}
