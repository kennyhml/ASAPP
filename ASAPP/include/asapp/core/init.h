#pragma once
#include "config.h"

namespace asa::core
{
    bool init(std::filesystem::path from_config);
    bool init(const std::filesystem::path& game_root_directory,
                         const std::filesystem::path& assets,
                         const std::filesystem::path& itemdata,
                         const std::filesystem::path& tessdata);
}
