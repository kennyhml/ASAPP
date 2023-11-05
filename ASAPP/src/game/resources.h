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
		bool InitItems();
		inline cv::Mat metal;
		inline cv::Mat metal_ingot;
		inline cv::Mat cementing_paste;
		inline cv::Mat crystal;
		inline cv::Mat fiber;
		inline cv::Mat flint;
		inline cv::Mat gunpowder;
		inline cv::Mat obsidian;
		inline cv::Mat polymer;
		inline cv::Mat stone;
		inline cv::Mat thatch;
		inline cv::Mat wood;
	}

	inline namespace icons
	{


	}




}
