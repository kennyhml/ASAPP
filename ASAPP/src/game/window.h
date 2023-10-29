#pragma once
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Windows.h>
#include <format>
#include <optional>
#include <string>
#include <vector>


namespace asa::window
{
	inline HWND hWnd = NULL;

	struct Point
	{
		int x{ 0 };
		int y{ 0 };
	};

	struct Color
	{
		int r{ 0 };
		int g{ 0 };
		int b{ 0 };

		Color ToBGR() const { return Color(b, g, r); }
		void ToRange(int variance, cv::Scalar& low, cv::Scalar& high) const;
	};

	struct Rect
	{
		int x{ 0 };
		int y{ 0 };
		int width{ 0 };
		int height{ 0 };

		const Point& GetEndPoint() const
		{
			return Point(x + width, y + height);
		}
	};

	std::optional<Rect> LocateTemplate(
		const Rect& region, const cv::Mat& templ, float threshold = 0.7);
	std::optional<Rect> LocateTemplate(
		const cv::Mat& source, const cv::Mat& templ, float threshold = 0.7);

	std::vector<Rect> LocateAllTemplate(
		const Rect& region, const cv::Mat& templ, float threshold = 0.7);
	std::vector<Rect> LocateAllTemplate(
		const cv::Mat& source, const cv::Mat& templ, float threshold = 0.7);

	bool MatchTemplate(
		const Rect& region, const cv::Mat& templ, float threshold = 0.7);
	bool MatchTemplate(
		const cv::Mat& source, const cv::Mat& templ, float threshold = 0.7);

	cv::Mat GetMask(const cv::Mat& image, const Color& color, float variance);
	cv::Mat GetMask(const Rect& region, const Color& color, float variance);

	void SetHandleTo(std::string title, int timeout = 60, bool verbose = 0);
	cv::Mat Screenshot(const Rect& region);

	inline std::ostream& operator<<(std::ostream& os, Point& point)
	{
		return os << std::format("Point(x={}, y={})", point.x, point.y);
	}

	inline std::ostream& operator<<(std::ostream& os, Color& c)
	{
		return os << std::format("Color(r={}, g={}, b={})", c.r, c.g, c.b);
	}

	inline std::ostream& operator<<(std::ostream& os, Rect& r)
	{
		return os << std::format("Rect(x={}, y={}, width={}, height={})", r.x,
				   r.y, r.width, r.height);
	}

	bool SetFocusedWindow();
	void SetMousePos(int x, int y);

}
