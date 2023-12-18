#pragma once
#include "iinterface.h"
#include "components/button.h"

namespace asa::interfaces
{
    class MainMenu final : public IInterface
    {
    private:
        components::Button esc{1732, 87, 82, 34};
        components::Button accept{764, 710, 376, 42};
        components::Button join_last_session_button{823, 924, 273, 51};
        components::Button start_button{772, 826, 377, 70};

    public:
        bool is_open() const override;
        bool got_connection_timeout() const;

        void accept_popup();
        void start();
        void join_last_session();
    };

    inline std::unique_ptr<MainMenu> main_menu = std::make_unique<MainMenu>();
}
