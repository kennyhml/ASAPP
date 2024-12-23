#include "asa/game/window.h"
#include "asa/utility.h"
#include "asa/core/state.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <opencv2/highgui.hpp>
#include <tesseract/baseapi.h>

namespace asa::window
{
    namespace
    {
        tesseract::TessBaseAPI* tesseract_engine = nullptr;

        HWND hwnd = nullptr;

        auto CRASH_WIN_TITLE = "The UE-ShooterGame Game has crashed and will close";


        // TODO: Paddings need to be tested on other resolutions
        constexpr auto WINDOWED_PADDING_TOP = 31;
        constexpr auto WINDOWED_PADDING = 8;

        BITMAPINFOHEADER get_bitmap_info_header(const int width, const int height)
        {
            BITMAPINFOHEADER bi;
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = width;
            bi.biHeight = -height;
            bi.biPlanes = 1;
            bi.biBitCount = 24;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0;
            bi.biXPelsPerMeter = 1;
            bi.biYPelsPerMeter = 2;
            bi.biClrUsed = 3;
            bi.biClrImportant = 4;
            return bi;
        }

        BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM lparam)
        {
            const auto windows = reinterpret_cast<std::map<std::string, HWND>*>(lparam);

            constexpr int max = 1024;
            char window_text[max];
            GetWindowTextA(hwnd, window_text, max);

            (*windows)[window_text] = hwnd;
            return true;
        }
    }

    void tesseract_init()
    {
        tesseract_engine = new tesseract::TessBaseAPI();

        //TODO: Bake the tessdata into the exe somehow, this is cringe!!!
        if (tesseract_engine->Init("", "eng")) {
            // INIT FAILED
            return;
        }
        // INIT SUCCESSFUL
    }

    cv::Mat screenshot(const cv::Rect& region, bool direct_capture)
    {
        if (hwnd && !IsWindow(hwnd)) { hwnd = nullptr; }

        // we cant do direct capture without a window handle
        direct_capture &= hwnd != nullptr;
        int32_t width, height;

        if (direct_capture) {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;

            if (width != 1920 || height != 1080) {
                return screenshot(region, false);
            }
        } else {
            width = region.width;
            height = region.height;
        }

        HDC dc = direct_capture ? GetWindowDC(hwnd) : GetDC(nullptr);
        HDC mdc = CreateCompatibleDC(dc);
        HBITMAP bitmap = CreateCompatibleBitmap(dc, width, height);

        SelectObject(mdc, bitmap);
        if (direct_capture) {
            PrintWindow(hwnd, mdc, PW_RENDERFULLCONTENT);
        } else { BitBlt(mdc, 0, 0, width, height, dc, region.x, region.y, SRCCOPY); }

        BITMAPINFOHEADER bi = get_bitmap_info_header(width, height);
        if (!direct_capture) { bi.biBitCount = 32; } // Use RGBA for BitBlt

        auto mat = cv::Mat(height, width, direct_capture ? CV_8UC3 : CV_8UC4);

        GetDIBits(mdc, bitmap, 0, height, mat.data,
                  reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

        DeleteObject(bitmap);
        DeleteDC(mdc);
        ReleaseDC(hwnd, dc);

        // after using BitBlt we have to drop the alpha channel
        if (!direct_capture) { cvtColor(mat, mat, cv::COLOR_RGBA2RGB); }

        // either the full screen was requested or we used BitBlt, which already only
        // captures the area of interest, so no cropping is needed.
        if ((region.width == 1920 && region.height == 1080) || !direct_capture) {
            return mat;
        }

        return mat(region);
    }

    cv::Vec3b pixel(const cv::Point& point)
    {
        HDC hdc = GetWindowDC(nullptr);
        COLORREF color = GetPixel(hdc, point.x, point.y);
        ReleaseDC(nullptr, hdc);

        return {GetRValue(color), GetGValue(color), GetBValue(color)};
    }

    void set_focus()
    {
        if (!hwnd) {
            throw std::exception("set_focus called without a known window handle");
        }
        SetForegroundWindow(hwnd);
    }

    void close()
    {
        auto crash = FindWindowExA(nullptr, nullptr, nullptr, CRASH_WIN_TITLE);
        if (!crash) { crash = FindWindowA(nullptr, "Crash!"); }

        if (crash) { PostMessageW(crash, WM_CLOSE, 0, 0); }
        PostMessageW(hwnd, WM_CLOSE, 0, 0);
    }

    std::optional<cv::Rect> locate(const cv::Mat& _template, const cv::Mat& source,
                                   const float threshold, const bool grayscale,
                                   const cv::Mat& mask, float* top,
                                   const int mode)
    {
        cv::Mat result;
        if (grayscale) {
            cv::Mat _template_gray, source_gray;
            cv::cvtColor(_template, _template_gray, cv::COLOR_RGB2GRAY);
            cv::cvtColor(source, source_gray, cv::COLOR_RGB2GRAY);

            if (mask.empty()) {
                matchTemplate(source, _template, result, mode);
            } else {
                cv::matchTemplate(source, _template, result, mode, mask);
            }
        } else {
            if (mask.empty()) {
                matchTemplate(source, _template, result, mode);
            } else {
                cv::matchTemplate(source, _template, result, mode, mask);
            }
        }


        double min_val, max_val;
        cv::Point min_loc, max_loc;
        cv::minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc);

        if (mode == cv::TM_SQDIFF || mode == cv::TM_SQDIFF_NORMED) {
            max_val = 1.0 - min_val;
            max_loc = min_loc;
        }

        if (top) { *top = static_cast<float>(max_val); }

        if (max_val < threshold) { return std::nullopt; }

        return cv::Rect(max_loc.x, max_loc.y, _template.cols, _template.rows);
    }

    std::optional<cv::Rect> locate(const cv::Mat& _template, const cv::Rect& region,
                                   const float threshold, const bool grayscale,
                                   const cv::Mat& mask, float* top,
                                   const int mode)
    {
        return locate(_template, screenshot(region), threshold, grayscale, mask, top,
                      mode);
    }

    std::vector<cv::Rect> locate_all(const cv::Mat& _template, const cv::Mat& source,
                                     const float threshold, const bool grayscale,
                                     const cv::Mat& mask)
    {
        cv::Mat match_result;
        cv::matchTemplate(source, _template, match_result, cv::TM_CCOEFF_NORMED,
                          mask);

        double min_val, max_val;
        cv::Point min_loc, max_loc;
        std::vector<cv::Rect> ret;

        while (true) {
            cv::minMaxLoc(match_result, &min_val, &max_val, &min_loc, &max_loc);
            if (max_val < threshold) { break; }

            cv::Rect loc{max_loc.x, max_loc.y, _template.cols, _template.rows};
            rectangle(match_result, {loc.x - 5, loc.y - 5, 15, 15}, {0}, cv::FILLED);
            ret.push_back(loc);
        }
        return ret;
    }

    std::vector<cv::Rect> locate_all(const cv::Mat& _template, const cv::Rect& region,
                                     const float threshold, const bool grayscale,
                                     const cv::Mat& mask)
    {
        return locate_all(_template, screenshot(region), threshold, grayscale, mask);
    }

    bool match(const cv::Mat& _template, const cv::Mat& source, const float threshold,
               const bool grayscale, const cv::Mat& mask)
    {
        return locate(_template, source, threshold, grayscale, mask) != std::nullopt;
    }

    bool match(const cv::Mat& _template, const cv::Rect& region,
               const float threshold,
               const bool grayscale, const cv::Mat& mask)
    {
        return locate(_template, screenshot(region), threshold, grayscale, mask) !=
               std::nullopt;
    }


    inline std::mutex ocr_mutex;

    std::string ocr_threadsafe(const cv::Mat& src, const tesseract::PageSegMode mode,
                               const char* whitelist)
    {
        // Mutex extremely important, tesseract engine is not threadsafe!!!
        std::lock_guard lock(ocr_mutex);

        tesseract_engine->SetImage(src.data, src.size().width, src.size().height,
                                   src.channels(), src.step1());

        tesseract_engine->SetPageSegMode(mode);
        tesseract_engine->SetVariable("tessedit_char_whitelist", whitelist);

        return tesseract_engine->GetUTF8Text();
    }

    void get_handle(int timeout, bool verbose)
    {
        using seconds = std::chrono::seconds;

        const auto start = std::chrono::system_clock::now();
        auto interval_start = start;
        bool info = false;

        do {
            auto now = std::chrono::system_clock::now();
            auto time_passed = std::chrono::duration_cast<seconds>(now - start);
            auto interval_passed = std::chrono::duration_cast<seconds>(
                now - interval_start);

            hwnd = FindWindowA("UnrealWindow", "ArkAscended");

            if (time_passed.count() > timeout && timeout != 0) {
                if (verbose) {
                    std::cout << "[!] Setting window handle failed." << std::endl;
                }
                return;
            }

            if (verbose && ((!hwnd && interval_passed.count() > 10) || !info)) {
                std::cout << "[+] Trying to find the window..." << std::endl;
                interval_start = now;
                info = true;
            }
        } while (!hwnd);
    }

    RECT get_window_rect()
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);

        if (get_user_setting<int>("FullscreenMode") == 0) {
            rect.left += WINDOWED_PADDING;
            rect.top += WINDOWED_PADDING_TOP;
            rect.right -= WINDOWED_PADDING;
            rect.bottom -= WINDOWED_PADDING;
        }

        return rect;
    }

    bool set_foreground()
    {
        return SetForegroundWindow(hwnd) != NULL;
    }

    bool has_crash_popup()
    {
        return FindWindowExA(nullptr, nullptr, nullptr, CRASH_WIN_TITLE)
               || FindWindowA(nullptr, "Crash!");
    }

    void set_mouse_pos(const cv::Point& location)
    {
        auto r = get_window_rect();
        SetCursorPos(location.x + r.left, location.y + r.top);
    }


    void click_at(const cv::Point& position, const controls::MouseButton button,
                  const std::chrono::milliseconds delay)
    {
        post_mouse_press_at(position, button);
        checked_sleep(delay);
    }

    void down(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        controls::down(input, delay);
    }

    void up(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        controls::release(input, delay);
    }

    void press(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        controls::press(input, delay);
    }

    void down(const std::string& key, const std::chrono::milliseconds delay)
    {
        controls::key_down(key, delay);
    }

    void up(const std::string& key, const std::chrono::milliseconds delay)
    {
        controls::key_up(key, delay);
    }

    void press(const std::string& key, const std::chrono::milliseconds delay)
    {
        controls::key_press(key, delay);
    }

    void post_down(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        controls::is_mouse_input(input)
            ? post_mouse_down(controls::str_to_button.at(input.key), delay)
            : post_key_down(input.key, delay);
    }

    void post_up(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        controls::is_mouse_input(input)
            ? post_mouse_up(controls::str_to_button.at(input.key), delay)
            : post_key_up(input.key, delay);
    }

    void post_press(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        if (controls::is_mouse_input(input)) {
            post_mouse_press(controls::str_to_button.at(input.key), delay);
        } else { post_key_press(input.key, delay); }
    }

    void post_key_down(const std::string& key, std::chrono::milliseconds delay)
    {
        PostMessageW(hwnd, WM_KEYDOWN, controls::get_virtual_keycode(key), NULL);
        checked_sleep(delay);
    }

    void post_key_up(const std::string& key, std::chrono::milliseconds delay)
    {
        PostMessageW(hwnd, WM_KEYUP, controls::get_virtual_keycode(key), NULL);
        checked_sleep(delay);
    }

    void post_key_press(const std::string& key, const std::chrono::milliseconds delay)
    {
        post_key_down(key);
        post_key_up(key);
        checked_sleep(delay);
    }

    void post_char(const char c) { PostMessageW(hwnd, WM_CHAR, c, NULL); }

    void post_mouse_down(const controls::MouseButton button,
                         const std::chrono::milliseconds delay)
    {
        using controls::MouseButton;
        switch (button) {
            case MouseButton::LEFT:
                PostMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, NULL);
                break;
            case MouseButton::RIGHT:
                PostMessageW(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, NULL);
                break;
            case MouseButton::MIDDLE:
                PostMessageW(hwnd, WM_MBUTTONDOWN, MK_MBUTTON, NULL);
                break;
            case MouseButton::MOUSE4:
                PostMessageW(hwnd, WM_XBUTTONDOWN, MK_XBUTTON1, NULL);
                break;
            case MouseButton::MOUSE5:
                PostMessageW(hwnd, WM_XBUTTONDOWN, MK_XBUTTON2, NULL);
                break;
        }
        checked_sleep(delay);
    }

    void post_mouse_up(const controls::MouseButton button,
                       const std::chrono::milliseconds delay)
    {
        using controls::MouseButton;
        switch (button) {
            case MouseButton::LEFT: PostMessageW(hwnd, WM_LBUTTONUP, MK_LBUTTON, NULL);
                break;
            case MouseButton::RIGHT: PostMessageW(hwnd, WM_RBUTTONUP, MK_RBUTTON, NULL);
                break;
            case MouseButton::MIDDLE: PostMessageW(hwnd, WM_MBUTTONUP, MK_MBUTTON, NULL);
                break;
            case MouseButton::MOUSE4: PostMessageW(hwnd, WM_XBUTTONUP, MK_XBUTTON1, NULL);
                break;
            case MouseButton::MOUSE5: PostMessageW(hwnd, WM_XBUTTONUP, MK_XBUTTON2, NULL);
                break;
        }
        checked_sleep(delay);
    }

    void post_mouse_press(const controls::MouseButton button,
                          const std::chrono::milliseconds delay)
    {
        post_mouse_down(button, delay);
        post_mouse_up(button);
        checked_sleep(delay);
    }

    void post_mouse_press_at(const cv::Point& position,
                             const controls::MouseButton button)
    {
        if (GetForegroundWindow() != hwnd) {
            set_foreground();
            checked_sleep(100ms);
        }

        LPARAM lParam = MAKELPARAM(position.x, position.y);
        if (button == controls::MouseButton::LEFT) {
            PostMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
        } else { PostMessageW(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam); }

        if (button == controls::MouseButton::LEFT) {
            PostMessageW(hwnd, WM_LBUTTONUP, MK_LBUTTON, lParam);
        } else { PostMessageW(hwnd, WM_RBUTTONUP, MK_RBUTTON, lParam); }
    }
}
