#pragma once
#include "interface.h"
#include "components/button.h"

namespace asa
{
    class main_menu final : public interface
    {
    public:
        bool is_open() const override;

        bool got_connection_timeout() const;

        void accept_popup();

        void start();

        void join_last_session();

    private:
        button esc{1732, 87, 82, 34};
        button accept{764, 710, 376, 42};
        button join_last_session_button{823, 924, 273, 51};
        button start_button{772, 826, 377, 70};
    };
}
