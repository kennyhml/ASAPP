#pragma once
#include "opencv2/imgproc.hpp"
#include <filesystem>
#include <opencv2/highgui/highgui.hpp>

namespace asa::resources
{
	inline std::filesystem::path assetsDir;

	bool Init();
	cv::Mat LoadResource(std::filesystem::path path);

	inline namespace interfaces
	{
		bool InitInterfaces();
		inline cv::Mat cb_arrowdown;
	}

	inline namespace items
	{

	}

	inline namespace icons
	{


	}




}
