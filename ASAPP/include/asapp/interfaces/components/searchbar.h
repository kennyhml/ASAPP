#pragma once

#include <string>
#include "iinterfacecomponent.h"

namespace asa::interfaces::components
{
    struct SearchBar : IInterfaceComponent
    {
    public:
        using IInterfaceComponent::IInterfaceComponent;

        virtual void press() const;

        virtual void search_for(std::string term);

        void delete_search();

        void set_text_cleared() { text_entered = false; }

        bool get_last_searched_term(std::string& term_out) const { return true; }

        /**
         * @brief Checks whether text is currently entered in the search bar.
         *
         * @remarks Takes up to 500ms to factor in the blinking cursor.
         *
         */
        [[nodiscard]] bool has_text_entered();

    protected:
        std::string last_searched_term = "";
        bool searching{false};
        bool text_entered{false};

        bool has_blinking_cursor() const;
    };
}
