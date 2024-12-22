#include "asa/structures/dedicatedstorage.h"
#include <asa/game/resources.h>

namespace asa::structures
{
    bool DedicatedStorage::can_deposit() const
    {
        return window::match_template(window::screenshot(),
                                      resources::interfaces::deposit);
    }
}
