#pragma once
#include "opencv2/imgproc.hpp"
#include <filesystem>
#include <opencv2/highgui/highgui.hpp>

namespace asa::resources
{

	bool Init();
	cv::Mat LoadResource(std::filesystem::path path);

	namespace interfaces
	{
		bool InitInterfaces();
		inline cv::Mat cb_arrowdown;
		inline cv::Mat regions;
		inline cv::Mat lay_on;
		inline cv::Mat day;
		inline cv::Mat esc;
		inline cv::Mat accept;
		inline cv::Mat back;
		inline cv::Mat join_last_session;
		inline cv::Mat refresh;

	}
	namespace text
	{
		bool InitText();
		inline cv::Mat added;
		inline cv::Mat removed;
		inline cv::Mat day;
		inline cv::Mat lootcrate;
		inline cv::Mat default_teleport;
		inline cv::Mat fast_travel;
		inline cv::Mat access_inventory;
		inline cv::Mat x;

	}

}
