#pragma once
#include <filesystem>

namespace asa::core::config
{
    bool set_environment(std::filesystem::path config_path);

    inline std::filesystem::path game_base_directory;
    inline std::filesystem::path assets_dir;
    inline std::filesystem::path itemdata_path;
    inline std::filesystem::path tessdata_path;

    bool set_game_directory(std::filesystem::path);
    bool set_assets_directory(std::filesystem::path);
    bool set_itemdata_path(std::filesystem::path);
    bool set_tessdata_path(std::filesystem::path);
}
