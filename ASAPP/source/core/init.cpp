#include "asapp/core/init.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/items/items.h"

bool asa::core::init(std::filesystem::path config_path)
{
    return (config::set_environment(config_path) && resources::init() && settings::init()
        && items::init() && window::init());
}
