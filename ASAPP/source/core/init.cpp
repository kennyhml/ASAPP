#include "asapp/core/init.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/items/items.h"

bool asa::core::init(std::filesystem::path config_path)
{
    return (config::set_environment(config_path) && resources::init() && settings::init()
        && items::init() && window::init());
}


bool asa::core::init(const std::filesystem::path& game_root_directory,
                     const std::filesystem::path& assets,
                     const std::filesystem::path& itemdata,
                     const std::filesystem::path& tessdata)
{
    return (config::set_environment(game_root_directory, assets, itemdata, tessdata) &&
        resources::init() && settings::init() && items::init() && window::init());
}
