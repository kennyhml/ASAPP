#include "asa/game/window.h"
#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/core/logging.h"
#include "asa/game/exceptions.h"

#include <chrono>
#include <fstream>
#include <random>
#include <tesseract/baseapi.h>

namespace asa
{
    namespace
    {
        using keyboard_mapping_t = std::unordered_map<std::string, int>;

        tesseract::TessBaseAPI* tesseract_engine = nullptr;
        HWND hwnd = nullptr;

        auto CRASH_WIN_TITLE = "The UE-ShooterGame Game has crashed and will close";

        // TODO: Paddings need to be tested on other resolutions
        constexpr auto WINDOWED_PADDING_TOP = 31;
        constexpr auto WINDOWED_PADDING = 8;

        constexpr float PIX_PER_DEGREE = 128.7f / 90.f;

        constexpr float MAX_LR_SENS = 3.2f;
        constexpr float MAX_UD_SENS = 3.2f;
        constexpr float MAX_FOV = 1.25f;

        std::mutex ocr_mutex;

        const keyboard_mapping_t base_keymap = {
            {"tab", VK_TAB}, {"f1", VK_F1}, {"f2", VK_F2}, {"f3", VK_F3}, {"f4", VK_F4},
            {"f5", VK_F5}, {"f6", VK_F6}, {"f7", VK_F7}, {"f8", VK_F8}, {"f9", VK_F9},
            {"f10", VK_F10}, {"delete", VK_DELETE}, {"home", VK_HOME}, {"end", VK_END},
            {"backspace", VK_BACK}, {"enter", VK_RETURN}, {"period", VK_OEM_PERIOD},
            {"numpadzero", VK_NUMPAD0}, {"numpadone", VK_NUMPAD1}, {"shift", VK_SHIFT},
            {"numpadtwo", VK_NUMPAD2}, {"numpadthree", VK_NUMPAD3},
            {"numpadfour", VK_NUMPAD4}, {"numpadfive", VK_NUMPAD5},
            {"numpadsix", VK_NUMPAD6}, {"numpadseven", VK_NUMPAD7},
            {"numpadeight", VK_NUMPAD8}, {"NumPadnine", VK_NUMPAD9}, {"ctrl", VK_CONTROL},
            {"esc", VK_ESCAPE}, {"space", VK_SPACE}, {"spacebar", VK_SPACE},
            {"zero", 0x30}, {"one", 0x31}, {"two", 0x32}, {"three", 0x33}, {"four", 0x34},
            {"five", 0x35}, {"six", 0x36}, {"seven", 0x37}, {"eight", 0x38},
            {"nine", 0x39},
            {"backspace", VK_BACK}, {"leftshift", VK_LSHIFT}, {"tilde", VK_OEM_3},
            {"comma", VK_OEM_COMMA}, {"up", VK_UP}
        };

        const auto str_to_button = std::unordered_map<std::string, MouseButton>{
            {"LeftMouseButton", MouseButton::LEFT},
            {"RightMouseButton", MouseButton::RIGHT},
            {"MiddleMouseButton", MouseButton::MIDDLE},
            {"ThumbMouseButton", MouseButton::MOUSE4},
            {"ThumbMouseButton2", MouseButton::MOUSE5},
        };

        keyboard_mapping_t get_keyboard_mapping()
        {
            keyboard_mapping_t mapping = base_keymap;

            for (int i = 32; i < 128; i++) {
                const char c = static_cast<char>(i);
                mapping[std::string(1, c)] = VkKeyScanA(c);
            }
            return mapping;
        }

        [[nodiscard]] int get_virtual_keycode(std::string key)
        {
            // dirty fix for now
            if (key == "Equals") {
                key = "=";
            } else if (key == "Backslash") {
                key = "\\";
            }

            utility::to_lower(key);
            static keyboard_mapping_t mapping = get_keyboard_mapping();
            return mapping.at(key);
        }

        [[nodiscard]] bool is_mouse_input(const action_mapping& input)
        {
            return input.key.contains("Mouse");
        }

        [[nodiscard]] float get_left_right_factor()
        {
            return MAX_LR_SENS / get_user_setting<float>("LookLeftRightSensitivity");
        }

        [[nodiscard]] float get_up_down_factor()
        {
            return MAX_UD_SENS / get_user_setting<float>("LookUpDownSensitivity");
        }

        [[nodiscard]] float get_fov_factor()
        {
            return MAX_FOV / get_user_setting<float>("FOVMultiplier");
        }

        void raw_input_key_down(const std::string& key)
        {
            INPUT input{};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = get_virtual_keycode(key);
            SendInput(1, &input, sizeof(INPUT));
        }

        void raw_input_key_up(const std::string& key)
        {
            INPUT input{};
            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            input.ki.wVk = get_virtual_keycode(key);
            SendInput(1, &input, sizeof(INPUT));
        }

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
    }

    void initialize_tesseract()
    {
        tesseract_engine = new tesseract::TessBaseAPI();
        if (tesseract_engine->Init("tessdata", "eng")) {
            throw tesseract_not_initialized("tessdata");
        }
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

    void set_window_focus()
    {
        SetForegroundWindow(hwnd);
    }

    void quit()
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

    HWND get_window_handle(const std::optional<std::chrono::seconds>& timeout)
    {
        const utility::stopwatch sw;
        get_logger()->info("Obtaining window handle..");

        hwnd = nullptr;
        do {
            hwnd = FindWindowA("UnrealWindow", "ArkAscended");
            checked_sleep(1s);
        } while (!hwnd && (timeout.has_value() && !sw.timedout(*timeout)));
        if (!hwnd) { throw window_not_found(); }

        get_logger()->info("Handle acquired: 0x{:x}.", reinterpret_cast<uintptr_t>(hwnd));
        return hwnd;
    }

    cv::Rect get_window_boundaries()
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);

        if (get_user_setting<int>("FullscreenMode") == 0) {
            rect.left += WINDOWED_PADDING;
            rect.top += WINDOWED_PADDING_TOP;
            rect.right -= WINDOWED_PADDING;
            rect.bottom -= WINDOWED_PADDING;
        }

        return {
            static_cast<int>(rect.left), static_cast<int>(rect.top),
            static_cast<int>(rect.right), static_cast<int>(rect.bottom)
        };
    }

    bool set_foreground()
    {
        return SetForegroundWindow(hwnd) != NULL;
    }

    bool is_hwnd_valid()
    {
        return IsWindow(hwnd);
    }

    void post_down(const action_mapping& action)
    {
        if (is_mouse_input(action)) {
            post_down(str_to_button.at(action.key));
        } else {
            post_down(action.key);
        }
    }

    void post_down(const MouseButton button, LPARAM params, const bool sleep)
    {
        switch (button) {
            case MouseButton::LEFT:
                PostMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, params);
                break;
            case MouseButton::RIGHT:
                PostMessageW(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, params);
                break;
            case MouseButton::MIDDLE:
                PostMessageW(hwnd, WM_MBUTTONDOWN, MK_MBUTTON, params);
                break;
            case MouseButton::MOUSE4:
                PostMessageW(hwnd, WM_XBUTTONDOWN, MK_XBUTTON1, params);
                break;
            case MouseButton::MOUSE5:
                PostMessageW(hwnd, WM_XBUTTONDOWN, MK_XBUTTON2, params);
                break;
        }
        if (sleep) {
            checked_sleep(10ms); // Prevents some quirks from PostMessage
        }
    }

    void post_down(const std::string& key)
    {
        PostMessageW(hwnd, WM_KEYDOWN, get_virtual_keycode(key), NULL);
        checked_sleep(10ms); // Prevents some quirks from PostMessage
    }

    void post_up(const action_mapping& action)
    {
        if (is_mouse_input(action)) {
            post_up(str_to_button.at(action.key));
        } else {
            post_up(action.key);
        }
    }

    void post_up(const MouseButton button, LPARAM params, const bool sleep)
    {
        switch (button) {
            case MouseButton::LEFT:
                PostMessageW(hwnd, WM_LBUTTONUP, MK_LBUTTON, params);
                break;
            case MouseButton::RIGHT:
                PostMessageW(hwnd, WM_RBUTTONUP, MK_RBUTTON, params);
                break;
            case MouseButton::MIDDLE:
                PostMessageW(hwnd, WM_MBUTTONUP, MK_MBUTTON, params);
                break;
            case MouseButton::MOUSE4:
                PostMessageW(hwnd, WM_XBUTTONUP, MK_XBUTTON1, params);
                break;
            case MouseButton::MOUSE5:
                PostMessageW(hwnd, WM_XBUTTONUP, MK_XBUTTON2, params);
                break;
        }
        if (sleep) {
            checked_sleep(10ms); // Prevents some quirks from PostMessage
        }
    }

    void post_up(const std::string& key)
    {
        PostMessageW(hwnd, WM_KEYUP, get_virtual_keycode(key), NULL);
        checked_sleep(10ms); // Prevents some quirks from PostMessage
    }

    void post_press(const action_mapping& action,
                    const std::chrono::milliseconds duration)
    {
        post_down(action);
        checked_sleep(duration);
        post_up(action);
    }

    void post_press(const MouseButton button, const std::optional<cv::Point>& location,
                    std::chrono::milliseconds duration)
    {
        LPARAM params = NULL;
        if (location) {
            params = MAKELPARAM(location->x, location->y);
        }
        post_down(button, params, !location.has_value());
        post_up(button, params, !location.has_value());
    }

    void post_press(const std::string& key, const std::chrono::milliseconds duration)
    {
        post_down(key);
        checked_sleep(duration);
        post_up(key);
    }

    void post_character(const char c)
    {
        PostMessageW(hwnd, WM_CHAR, c, NULL);
    }

    void typewrite(const std::string& text)
    {
        for (const char c: text) { post_character(c); }
    }

    void post_combination(const std::string& down, const std::string& press)
    {
        raw_input_key_down(down);
        raw_input_key_down(press);
        raw_input_key_up(press);
        raw_input_key_up(down);
    }

    void turn(const int x, const int y)
    {
        INPUT input{};
        input.type = INPUT_MOUSE;

        input.mi.dx = static_cast<LONG>(std::round(
            x * PIX_PER_DEGREE * get_left_right_factor() * get_fov_factor()));
        input.mi.dy = static_cast<LONG>(std::round(
            y * PIX_PER_DEGREE * get_up_down_factor() * get_fov_factor()));
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

        SendInput(1, &input, sizeof(INPUT));
    }

    void turn_to(int x, int y)
    {
        INPUT input{0};
        input.type = INPUT_MOUSE;
        int moveX = (x - GetSystemMetrics(SM_CXSCREEN) / 2);
        int moveY = (y - GetSystemMetrics(SM_CYSCREEN) / 2);

        input.mi.dx = moveX;
        input.mi.dy = moveY;
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_MOVE_NOCOALESCE;

        SendInput(1, &input, sizeof(INPUT));
    }

    bool has_crash_popup()
    {
        return FindWindowA(nullptr, CRASH_WIN_TITLE) || FindWindowA(nullptr, "Crash!");
    }

    void set_mouse_pos(const cv::Point& location)
    {
        const auto rect = get_window_boundaries();
        SetCursorPos(location.x + rect.x, location.y + rect.y);
    }
}
