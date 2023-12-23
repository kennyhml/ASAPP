#pragma once
#include "components/button.h"
#include "components/searchbar.h"

#include "iinterface.h"

namespace asa::interfaces
{
    class ServerSelect final : public IInterface
    {
    private:
        components::SearchBar searchbar{1577, 172, 184, 46};

        window::Rect joining_text{717, 469, 507, 38};
        window::Rect joining_text_2{828, 357, 266, 66};

        components::Button official_button{571, 182, 116, 49};
        components::Button refresh_button{834, 932, 275, 39};
        components::Button best_result{94, 306, 1730, 43};
        components::Button join_button_mods_popup{266, 911, 162, 39}; // The join button if the server has mods enabled
        components::Button join_button{1615, 924, 195, 41};
        components::Button join_last_played_button{1624, 876, 178, 39};

        bool is_best_result_selected();

    public:
        bool is_open() const override;
        bool can_join() const;
        bool can_join_last_played() const;
        bool is_joining_server() const;
        bool server_has_mods_enabled() const;

        void join_server(const std::string& serverName);
        void refresh();
    };


    inline std::unique_ptr<ServerSelect> server_select = std::make_unique<ServerSelect>();
}
