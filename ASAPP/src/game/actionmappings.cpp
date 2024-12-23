#include "asa/game/actionmappings.h"
#include <format>

namespace asa::settings
{
    action_mapping::action_mapping(std::string t_name,
                                   std::string t_default) : name(std::move(t_name)),
        key(std::move(t_default)) { mapped[name] = this; };

    std::string action_mapping::to_string() const
    {
        return std::format(
            "ActionMapping(name={}, shift={}, " "ctrl={}, alt={}, cmd={}, key={})",
            name, static_cast<int>(shift), static_cast<int>(ctrl),
            static_cast<int>(alt), static_cast<int>(cmd), key);
    }
}
