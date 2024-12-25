#pragma once
#include "components/button.h"
#include "components/search_bar.h"
#include "asainterface.h"

namespace asa
{
    class server_select final : public asainterface
    {
    public:
        [[nodiscard]] bool is_open() const override;
        [[nodiscard]] bool can_join() const;
        [[nodiscard]] bool can_join_last_played() const;
        [[nodiscard]] bool is_joining_server() const;
        [[nodiscard]] bool server_has_mods_enabled() const;

        void join_server(const std::string& name);

        void join_last_played();

        void refresh();

    private:
        search_bar searchbar_{1577, 172, 184, 46};

        cv::Rect joining_text_{717, 469, 507, 20};
        cv::Rect joining_text_2_{828, 357, 266, 66};

        button official_button_{571, 182, 116, 49};
        button refresh_button_{834, 932, 275, 39};
        button best_result_{94, 306, 1730, 43};

        // The join button if the server has mods enabled
        button join_button_mods_popup_{447, 920, 50, 31};
        button join_button_{1615, 924, 195, 41};
        button join_last_played_button_{1624, 876, 178, 39};

        [[nodiscard]] bool is_best_result_selected() const;
    };
}
