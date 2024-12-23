#include "asa/structures/dedicated_storage.h"
#include <asa/game/resources.h>

namespace asa::structures
{
    bool dedicated_storage::can_deposit() const
    {
        return window::match_template(window::screenshot(),
                                      resources::interfaces::deposit);
    }
}
