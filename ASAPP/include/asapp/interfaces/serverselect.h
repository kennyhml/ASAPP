#pragma once
#include "components/button.h"
#include "components/searchbar.h"

#include "iinterface.h"

namespace asa::interfaces
{
    class ServerSelect final : public IInterface
    {
    private:
        components::SearchBar searchbar_{1577, 172, 184, 46};

        window::Rect joining_text_{717, 469, 507, 38};
        window::Rect joining_text_2_{828, 357, 266, 66};

        components::Button official_button_{571, 182, 116, 49};
        components::Button refresh_button_{834, 932, 275, 39};
        components::Button best_result_{94, 306, 1730, 43};
        // The join button if the server has mods enabled
        components::Button join_button_mods_popup_{266, 911, 162, 39};
        components::Button join_button_{1615, 924, 195, 41};
        components::Button join_last_played_button_{1624, 876, 178, 39};

        [[nodiscard]] bool is_best_result_selected() const;

    public:
        [[nodiscard]] bool is_open() const override;
        [[nodiscard]] bool can_join() const;
        [[nodiscard]] bool can_join_last_played() const;
        [[nodiscard]] bool is_joining_server() const;
        [[nodiscard]] bool server_has_mods_enabled() const;

        void join_server(const std::string& name);
        void refresh();
    };


    inline std::unique_ptr<ServerSelect> server_select = std::make_unique<ServerSelect>();
}
