#pragma once
#include "iinterface.h"
#include "components/button.h"

namespace asa::interfaces
{
    class ModeSelect final : public IInterface
    {
    private:
        components::Button join_game_button{540, 247, 383, 640};
        components::Button create_or_resume_game_button{997, 251, 382, 634};
        components::Button mods_list_button{1449, 247, 396, 647};
        components::Button back_button{893, 946, 135, 36};

    public:
        bool is_open() const override;

        void join_game();
    };


    inline std::unique_ptr<ModeSelect> mode_select = std::make_unique<ModeSelect>();
}
