#pragma once
#define WIN32_LEAN_AND_MEAN

#include "controls.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Windows.h>
#include <format>
#include <optional>
#include <string>
#include <tesseract/baseapi.h>
#include <vector>

namespace asa::window
{
	bool Init();

	using ms = std::chrono::milliseconds;

	inline tesseract::TessBaseAPI* tessEngine = nullptr;
	inline HWND hWnd = NULL;
	inline int width = NULL;
	inline int height = NULL;

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
		Rect(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height){};

		int x{ 0 };
		int y{ 0 };
		int width{ 0 };
		int height{ 0 };

		const Point& GetEndPoint() const
		{
			return Point(x + width, y + height);
		}

		const Point GetRandLocation(int padding) const;
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

	void GetHandle(int timeout = 60, bool verbose = 0);
	cv::Mat Screenshot(const Rect& region = Rect(0, 0, 0, 0));

	void SetTesseractImage(const cv::Mat& image);

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

	bool SetForeground();
	bool SetForegroundButHidden();
	bool HasCrashedPopup();

	void SetMousePos(const Point&);
	void SetMousePos(int x, int y);
	void ClickAt(const Point&, controls::MouseButton, ms delay = ms(50));

	void Down(const settings::ActionMapping&, ms delay = ms(10));
	void Up(const settings::ActionMapping&, ms delay = ms(10));
	void Press(const settings::ActionMapping&, bool catchCursor = false,
		ms delay = ms(50));

	void Down(const std::string& key, ms delay = ms(0));
	void Up(const std::string& key, ms delay = ms(0));
	void Press(
		const std::string& key, bool catchCursor = false, ms delay = ms(0));

	void PostDown(const settings::ActionMapping&, ms delay = ms(10));
	void PostUp(const settings::ActionMapping&, ms delay = ms(10));
	void PostPress(const settings::ActionMapping&, bool catchCursor = false,
		ms delay = ms(50));

	void PostKeyDown(const std::string& key, ms delay = ms(0));
	void PostKeyUp(const std::string& key, ms delay = ms(0));
	void PostKeyPress(
		const std::string& key, bool catchCursor = false, ms delay = ms(0));

	void PostChar(char c);

	void PostMouseDown(controls::MouseButton, ms delay = ms(10));
	void PostMouseUp(controls::MouseButton, ms delay = ms(10));
	void PostMousePress(
		controls::MouseButton, bool catchCursor = false, ms delay = ms(100));
	void PostMousePressAt(const Point&, controls::MouseButton);

	void ResetCursor(POINT& previousPosition);


}
