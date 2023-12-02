#pragma once
#include <filesystem>

namespace asa::config
{
	bool Init(std::filesystem::path configPath);

	inline std::filesystem::path gameBaseDirectory;
	inline std::filesystem::path assetsDir;
	inline std::filesystem::path itemdataPath;
	inline std::filesystem::path tessdataPath;

	bool SetGameDirectory(std::filesystem::path);
	bool SetAssetsDirectory(std::filesystem::path);
	bool SetItemdataPath(std::filesystem::path);
	bool SetTessdataPath(std::filesystem::path);
}
