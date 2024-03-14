#include "asapp/structures/dedicatedstorage.h"
#include <asapp/game/resources.h>

namespace asa::structures
{
    bool DedicatedStorage::can_deposit() const
    {
        return window::match_template(window::screenshot(),
                                      resources::interfaces::deposit);
    }
}
