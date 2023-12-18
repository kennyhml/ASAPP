#pragma once
#include "basestructure.h"
#include "asapp/game/settings.h"
#include "asapp/interfaces/iinterface.h"

namespace asa::structures
{
    class InteractableStructure : public BaseStructure
    {
    protected:
        const settings::ActionMapping* interact_key;

    public:
        InteractableStructure(std::string name,
                              const settings::ActionMapping* t_interact_key,
                              interfaces::IInterface* t_interface) : BaseStructure(name),
            interact_key(t_interact_key), _interface(t_interface) {};

        interfaces::IInterface* _interface;

        const settings::ActionMapping get_interact_key() const { return *(interact_key); }
    };
}
