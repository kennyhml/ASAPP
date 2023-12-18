#pragma once
#include "iinterface.h"
#include "asapp/game/window.h"
#include "components/button.h"

namespace asa::interfaces
{
    class InterfaceManager : IInterface
    {
        struct TabButton : components::Button
        {
            TabButton(int x) : Button(x, 53, 47, 48) {};

            using Button::Button;
            bool is_selected() const;
        };

    public:
        components::Button close_button{1784, 50, 32, 31};

        TabButton inventory_tab_button{794};
        TabButton engrams_tab_button{851};
        TabButton tribe_tab_button{908};
        TabButton tracking_tab_button{965};
        TabButton notes_tab_button{1022};
        TabButton map_tab_button{1079};

        IInterface* get_open_interface() const;

        bool is_open() const override;
    };

    inline InterfaceManager* gInterfaceManager = new InterfaceManager();
}
