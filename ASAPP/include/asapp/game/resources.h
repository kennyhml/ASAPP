#pragma once
#include "opencv2/imgproc.hpp"
#include <filesystem>
#include <opencv2/highgui/highgui.hpp>

namespace asa::resources
{

	bool Init();
	cv::Mat LoadResource(std::filesystem::path path);

	inline namespace interfaces
	{
		bool InitInterfaces();
		inline cv::Mat cb_arrowdown;
		inline cv::Mat regions;
	}

	inline namespace items
	{
		bool InitItems();
		inline cv::Mat metal;
		inline cv::Mat metal_ingot;
		inline cv::Mat paste;
		inline cv::Mat crystal;
		inline cv::Mat fiber;
		inline cv::Mat flint;
		inline cv::Mat gunpowder;
		inline cv::Mat sparkpowder;
		inline cv::Mat obsidian;
		inline cv::Mat polymer;
		inline cv::Mat stone;
		inline cv::Mat thatch;
		inline cv::Mat wood;
		inline cv::Mat gasmask;
	}

	inline namespace text
	{
		bool InitText();
		inline cv::Mat added;
		inline cv::Mat removed;
		inline cv::Mat day;
		inline cv::Mat lootcrate;
	}

}
