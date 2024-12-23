#include "asa/core/init.h"
#include "../../include/asa/game/settings.h"
#include "asa/game/window.h"
#include "asa/items/items.h"
#include "asa/game/resources.h"

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
