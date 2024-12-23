#pragma once
#include "interface.h"
#include "components/button.h"

namespace asa
{
    class mode_select final : public interface
    {
    private:
        button join_game_button{540, 247, 383, 640};
        button create_or_resume_game_button{997, 251, 382, 634};
        button mods_list_button{1449, 247, 396, 647};
        button back_button{893, 946, 135, 36};

    public:
        bool is_open() const override;

        void join_game();
    };
}
