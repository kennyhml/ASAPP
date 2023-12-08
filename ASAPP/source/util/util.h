#pragma once
#include "opencv2/imgproc.hpp"
#include <chrono>
#include <functional>

namespace util
{
	bool Await(const std::function<bool()>& condition,
		std::chrono::milliseconds timeout);

	bool Timedout(const std::chrono::system_clock::time_point& start,
		const std::chrono::milliseconds timeout);

	bool Timedout(const std::chrono::system_clock::time_point& start,
		const std::chrono::seconds timeout);

	template <typename Type> const char* GetName(const Type& obj)
	{
		return typeid(obj).name();
	}

	bool IsOnlyOneBitSet(int bitfield);
	void SetClipboard(const std::string& term);

	cv::Mat MaskAlphaChannel(const cv::Mat& src);
}
