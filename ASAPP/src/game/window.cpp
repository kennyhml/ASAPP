#include "window.h"
#include <chrono>
#include <iostream>

using namespace asa;

void window::Color::ToRange(int v, cv::Scalar& low, cv::Scalar& high) const
{
	low = cv::Scalar(max(0, r - v), max(0, g - v), max(0, b - v));
	high = cv::Scalar(min(255, r + v), min(255, g + v), min(255, b + v));
}

std::optional<window::Rect> window::LocateTemplate(
	const Rect& region, const cv::Mat& templ, float threshold)
{
	cv::Mat image = Screenshot(region);
	return LocateTemplate(image, templ, threshold);
}

std::optional<window::Rect> window::LocateTemplate(
	const cv::Mat& source, const cv::Mat& templ, float threshold)
{
	cv::Mat result;
	cv::matchTemplate(source, templ, result, cv::TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

	if (maxVal < threshold) {
		return std::nullopt;
	}

	return Rect(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
}

std::vector<window::Rect> window::LocateAllTemplate(
	const Rect& region, const cv::Mat& templ, float threshold)
{
	cv::Mat image = Screenshot(region);
	return LocateAllTemplate(image, templ, threshold);
}

std::vector<window::Rect> window::LocateAllTemplate(
	const cv::Mat& source, const cv::Mat& templ, float threshold)
{

	cv::Mat matchResult;
	cv::matchTemplate(source, templ, matchResult, cv::TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	std::vector<window::Rect> allMatches;

	while (true) {
		cv::minMaxLoc(matchResult, &minVal, &maxVal, &minLoc, &maxLoc);
		if (maxVal < threshold) {
			break;
		}

		window::Rect loc{ maxLoc.x, maxLoc.y, templ.cols, templ.rows };
		cv::rectangle(matchResult, { loc.x - 5, loc.y - 5, 15, 15 },
			cv::Scalar(0, 0, 0), cv::FILLED);

		allMatches.push_back(loc);
	}
	return allMatches;
}

bool window::MatchTemplate(
	const Rect& region, const cv::Mat& templ, float threshold)
{
	return LocateTemplate(region, templ, threshold) != std::nullopt;
}

bool window::MatchTemplate(
	const cv::Mat& source, const cv::Mat& templ, float threshold)
{
	return LocateTemplate(source, templ, threshold) != std::nullopt;
}

cv::Mat window::GetMask(const Rect& region, const Color& color, float variance)
{
	cv::Mat image = Screenshot(region);
	return GetMask(image, color, variance);
}

cv::Mat window::GetMask(
	const cv::Mat& image, const Color& color, float variance)
{
	cv::Scalar low;
	cv::Scalar high;
	color.ToBGR().ToRange(variance, low, high);

	cv::Mat mask;
	cv::inRange(image, low, high, mask);
	return mask;
}

void window::SetHandleTo(std::string title, int timeout, bool verbose)
{
	using seconds = std::chrono::seconds;

	auto start = std::chrono::system_clock::now();
	auto intervalStart = start;
	bool firstGrab = !hWnd;
	bool info = false;

	do {
		auto now = std::chrono::system_clock::now();
		auto timePassed = std::chrono::duration_cast<seconds>(now - start);
		auto intervalPassed = std::chrono::duration_cast<seconds>(
			now - intervalStart);

		hWnd = FindWindowA(NULL, title.c_str());

		if (timePassed.count() > timeout && timeout != 0) {
			if (verbose) {
				std::cout << "[!] Setting window handle failed." << std::endl;
			}
			return;
		}

		if (verbose && ((!hWnd && intervalPassed.count() > 10) || !info)) {
			std::cout << "[+] Trying to find '" << title << "'..." << std::endl;
			intervalStart = now;
			info = true;
		}
	} while (!hWnd);

	if (firstGrab && verbose) {
		std::cout << "\t[-] Set window handle: " << title << "!" << std::endl;
	}
}

static BITMAPINFOHEADER& GetBitmapInfoHeader(
	int width, int height, int bitCount, int compression)
{
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = bitCount;
	bi.biCompression = compression;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 1;
	bi.biYPelsPerMeter = 2;
	bi.biClrUsed = 3;
	bi.biClrImportant = 4;

	return bi;
}

static HBITMAP& GetBitmap(const window::Rect& region, HDC& memoryDeviceContext)
{
	HDC deviceContext = GetDC(NULL);
	memoryDeviceContext = CreateCompatibleDC(deviceContext);
	HBITMAP bitmap = CreateCompatibleBitmap(
		deviceContext, region.width, region.height);

	SelectObject(memoryDeviceContext, bitmap);
	BitBlt(memoryDeviceContext, 0, 0, region.width, region.height,
		deviceContext, region.x, region.y, SRCCOPY);

	// Can release it now, avoids making another function take its
	// reponsibility.
	ReleaseDC(NULL, deviceContext);
	return bitmap;
}

cv::Mat window::Screenshot(const Rect& region)
{
	HDC mDc;
	HBITMAP bitmap = GetBitmap(region, mDc);
	BITMAPINFOHEADER bi = GetBitmapInfoHeader(
		region.width, region.height, 32, BI_RGB);

	// copies the bitmap into the mat buffer
	cv::Mat mat = cv::Mat(region.height, region.width, CV_8UC4);
	GetDIBits(mDc, bitmap, 0, region.height, mat.data,
		reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

	DeleteObject(bitmap);
	DeleteDC(mDc);

	// convert to RGB since RGBA is fucking useless, all our templates are gonna
	// be in RGB
	cv::Mat result;
	cv::cvtColor(mat, result, cv::COLOR_RGBA2RGB);

	return result;
}

bool window::SetForeground()
{
	if (!hWnd) {
		std::cout << "[!] Cant focus window, get the hWnd first." << std::endl;
		return false;
	}
	return SetForegroundWindow(hWnd) != NULL;
}

void window::SetMousePos(int x, int y)
{
	// do some checks here later that x and y is actually within the boudaries
	// of the game
	SetCursorPos(x, y);
}

