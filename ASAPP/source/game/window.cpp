#include "asapp/game/window.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/core/config.h"
#include "asapp/game/globals.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace asa;

bool window::Init()
{
	tessEngine = new tesseract::TessBaseAPI();

	if (tessEngine->Init(config::tessdataPath.string().c_str(), "eng")) {
		std::cerr << "[!] Failed to initialize tesseract!" << std::endl;
		return false;
	}
	std::cout << "[+] Tesseract engine initialized successfully." << std::endl;
}

void window::Color::ToRange(int v, cv::Scalar& low, cv::Scalar& high) const
{
	low = cv::Scalar(
		std::max(0, r - v), std::max(0, g - v), std::max(0, b - v));
	high = cv::Scalar(
		std::min(255, r + v), std::min(255, g + v), std::min(255, b + v));
}

const window::Point window::Rect::GetRandLocation(int padding) const
{
	const int xMin = padding;
	const int xMax = this->width - padding;

	const int yMin = padding;
	const int yMax = this->height - padding;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randXRange(xMin, xMax);
	std::uniform_int_distribution<int> randYRange(yMin, yMax);

	return Point{ this->x + randXRange(gen), this->y + randYRange(gen) };
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
	CheckState();

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

void window::SetTesseractImage(const cv::Mat& image)
{
	tessEngine->SetImage(image.data, image.size().width, image.size().height,
		image.channels(), image.step1());
}

void window::GetHandle(int timeout, bool verbose)
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

		hWnd = FindWindowA("UnrealWindow", "ArkAscended");

		if (timePassed.count() > timeout && timeout != 0) {
			if (verbose) {
				std::cout << "[!] Setting window handle failed." << std::endl;
			}
			return;
		}

		if (verbose && ((!hWnd && intervalPassed.count() > 10) || !info)) {
			std::cout << "[+] Trying to find the window..." << std::endl;
			intervalStart = now;
			info = true;
		}
	} while (!hWnd);

	RECT rect;
	GetWindowRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	if (firstGrab && verbose) {
		std::cout << std::format(
						 "\t[-] Set window handle! Width: {}, height: {}",
						 width, height)
				  << std::endl;
	}
}

static BITMAPINFOHEADER GetBitmapInfoHeader(
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

cv::Mat window::Screenshot(const Rect& region)
{
	SetProcessDPIAware();

	RECT rect;
	GetWindowRect(hWnd, &rect);
	int wWidth = rect.right - rect.left;
	int wHeight = rect.bottom - rect.top;

	HDC hwndDC = GetWindowDC(hWnd);
	HDC mDc = CreateCompatibleDC(hwndDC);
	HBITMAP bitmap = CreateCompatibleBitmap(hwndDC, wWidth, wHeight);

	SelectObject(mDc, bitmap);
	PrintWindow(hWnd, mDc, PW_RENDERFULLCONTENT);

	BITMAPINFOHEADER bi = GetBitmapInfoHeader(wWidth, wHeight, 32, BI_RGB);

	// copies the bitmap into the mat buffer
	cv::Mat mat = cv::Mat(wHeight, wWidth, CV_8UC4);
	GetDIBits(mDc, bitmap, 0, wHeight, mat.data,
		reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

	DeleteObject(bitmap);
	DeleteDC(mDc);
	ReleaseDC(hWnd, hwndDC);

	// convert to RGB since RGBA is useless
	cv::Mat result;
	cv::cvtColor(mat, result, cv::COLOR_RGBA2RGB);

	if (!region.width && !region.height) {
		return result;
	}
	return result(cv::Rect(region.x, region.y, region.width, region.height))
		.clone();
}

bool window::SetForeground()
{
	if (!hWnd) {
		std::cout << "[!] Cant focus window, get the hWnd first." << std::endl;
		return false;
	}
	return SetForegroundWindow(hWnd) != NULL;
}

bool window::SetForegroundButHidden()
{
	return SetForeground() && SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0,
								  SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void window::SetMousePos(const Point& location)
{
	SetCursorPos(location.x, location.y);
}

void window::SetMousePos(int x, int y) { SetCursorPos(x, y); }

void window::ClickAt(
	const Point& position, controls::MouseButton button, ms delay)
{

	if (!globals::useWindowInput) {
		SetMousePos(position);
		SleepFor(delay);
		controls::MousePress(button);
	}
	else {
		window::PostMousePressAt(position, button);
	}
}

void window::Down(const settings::ActionMapping& input, ms delay)
{
	globals::useWindowInput ? PostDown(input, delay)
							: controls::Down(input, delay);
}

void window::Up(const settings::ActionMapping& input, ms delay)
{
	globals::useWindowInput ? PostUp(input, delay)
							: controls::Release(input, delay);
}

void window::Press(
	const settings::ActionMapping& input, bool catchCursor, ms delay)
{
	globals::useWindowInput ? PostPress(input, catchCursor, delay)
							: controls::Press(input, delay);
}

void window::Down(const std::string& key, ms delay)
{
	globals::useWindowInput ? PostKeyDown(key, delay)
							: controls::KeyDown(key, delay);
}

void window::Up(const std::string& key, ms delay)
{
	globals::useWindowInput ? PostKeyUp(key, delay)
							: controls::KeyUp(key, delay);
}

void window::Press(const std::string& key, bool catchCursor, ms delay)
{
	globals::useWindowInput ? PostKeyPress(key, catchCursor, delay)
							: controls::KeyPress(key, delay);
}

void window::PostDown(const settings::ActionMapping& input, ms delay)
{
	controls::IsMouseInput(input)
		? PostMouseDown(controls::stringToMouseButton[input.key], delay)
		: PostKeyDown(input.key, delay);
}

void window::PostUp(const settings::ActionMapping& input, ms delay)
{
	controls::IsMouseInput(input)
		? PostMouseUp(controls::stringToMouseButton[input.key], delay)
		: PostKeyUp(input.key, delay);
}

void window::PostPress(
	const settings::ActionMapping& input, bool catchCursor, ms delay)
{
	if (controls::IsMouseInput(input)) {
		PostMousePress(
			controls::stringToMouseButton[input.key], catchCursor, delay);
	}
	else {
		PostKeyPress(input.key, catchCursor, delay);
	}
}

void window::PostKeyDown(const std::string& key, ms delay)
{
	PostMessageW(hWnd, WM_KEYDOWN, controls::GetVirtualKeyCode(key), NULL);
	SleepFor(delay);
}

void window::PostKeyUp(const std::string& key, ms delay)
{
	PostMessageW(hWnd, WM_KEYUP, controls::GetVirtualKeyCode(key), NULL);
	SleepFor(delay);
}

void window::PostKeyPress(const std::string& key, bool catchCursor, ms delay)
{
	POINT prevPos;
	if (catchCursor) {
		GetCursorPos(&prevPos);
	}

	PostKeyDown(key);
	PostKeyUp(key);

	if (catchCursor) {
		ResetCursor(prevPos);
	}
}

void window::PostChar(char c) { PostMessageW(hWnd, WM_CHAR, c, NULL); }

void window::PostMouseDown(controls::MouseButton button, ms delay)
{
	using controls::MouseButton;
	switch (button) {
	case MouseButton::LEFT:
		PostMessageW(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, NULL);
		break;
	case MouseButton::RIGHT:
		PostMessageW(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, NULL);
		break;
	case MouseButton::MIDDLE:
		PostMessageW(hWnd, WM_MBUTTONDOWN, MK_MBUTTON, NULL);
		break;
	case MouseButton::MOUSE4:
		PostMessageW(hWnd, WM_XBUTTONDOWN, MK_XBUTTON1, NULL);
		break;
	case MouseButton::MOUSE5:
		PostMessageW(hWnd, WM_XBUTTONDOWN, MK_XBUTTON2, NULL);
		break;
	}
	SleepFor(delay);
}

void window::PostMouseUp(controls::MouseButton button, ms delay)
{
	using controls::MouseButton;
	switch (button) {
	case MouseButton::LEFT:
		PostMessageW(hWnd, WM_LBUTTONUP, MK_LBUTTON, NULL);
		break;
	case MouseButton::RIGHT:
		PostMessageW(hWnd, WM_RBUTTONUP, MK_RBUTTON, NULL);
		break;
	case MouseButton::MIDDLE:
		PostMessageW(hWnd, WM_MBUTTONUP, MK_MBUTTON, NULL);
		break;
	case MouseButton::MOUSE4:
		PostMessageW(hWnd, WM_XBUTTONUP, MK_XBUTTON1, NULL);
		break;
	case MouseButton::MOUSE5:
		PostMessageW(hWnd, WM_XBUTTONUP, MK_XBUTTON2, NULL);
		break;
	}
	SleepFor(delay);
}

void window::PostMousePress(
	controls::MouseButton button, bool catchCursor, ms delay)
{
	POINT prevPos;
	if (catchCursor) {
		GetCursorPos(&prevPos);
	}

	PostMouseDown(button, delay);
	PostMouseUp(button);

	if (catchCursor) {
		ResetCursor(prevPos);
	}
}

void window::PostMousePressAt(
	const Point& position, controls::MouseButton button)
{
	HWND previousFocus = GetForegroundWindow();
	SetForegroundButHidden();
	SleepFor(ms(50));

	LPARAM lParam = MAKELPARAM(position.x, position.y);
	if (button == controls::MouseButton::LEFT) {
		PostMessageW(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
	}
	else {
		PostMessageW(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
	}

	if (button == controls::MouseButton::LEFT) {
		PostMessageW(hWnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
	}
	else {
		PostMessageW(hWnd, WM_RBUTTONUP, MK_RBUTTON, lParam);
	}
	SetForegroundWindow(previousFocus);
}

void window::ResetCursor(POINT& previousPosition)
{
	auto start = std::chrono::system_clock::now();
	POINT currPos;
	GetCursorPos(&currPos);

	while (!util::Timedout(start, ms(250))) {

		while (currPos.x != width / 2 && currPos.y != height / 2) {
			previousPosition = currPos;
			GetCursorPos(&currPos);
		}
		SetCursorPos(previousPosition.x, previousPosition.y);
	}
}
