#pragma once
#define WIN32_LEAN_AND_MEAN

#include <format>
#include <optional>
#include <string>
#include <vector>
#include <Windows.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include "controls.h"


namespace asa::window
{
    bool init();

    inline tesseract::TessBaseAPI* tessEngine = nullptr;
    inline HWND hWnd = nullptr;
    inline int width = NULL;
    inline int height = cv::countNonZero(cv::Mat());;

    struct Point
    {
        int x{0};
        int y{0};
    };

    struct Color
    {
        int r{0};
        int g{0};
        int b{0};

        Color to_bgr() const { return Color(b, g, r); }
        void to_range(int variance, cv::Scalar& low, cv::Scalar& high) const;
    };

    struct Rect
    {
        Rect(int x, int y, int width, int height) : x(x), y(y), width(width),
                                                    height(height) {};

        int x{0};
        int y{0};
        int width{0};
        int height{0};

        const Point& get_end_point() const { return Point(x + width, y + height); }

        const Point get_random_location(int padding) const;
    };

    std::optional<Rect> locate_template(const Rect& region, const cv::Mat& templ,
                                        float threshold = 0.7,
                                        const cv::Mat& mask = cv::Mat());
    std::optional<Rect> locate_template(const cv::Mat& source, const cv::Mat& templ,
                                        float threshold = 0.7,
                                        const cv::Mat& mask = cv::Mat(),
                                        float* highest_match = nullptr,
                                        int mode = cv::TM_CCOEFF_NORMED);

    std::vector<Rect> locate_all_template(const Rect& region, const cv::Mat& templ,
                                          float threshold = 0.7,
                                          const cv::Mat& mask = cv::Mat());
    std::vector<Rect> locate_all_template(const cv::Mat& source, const cv::Mat& templ,
                                          float threshold = 0.7,
                                          const cv::Mat& mask = cv::Mat());

    bool match_template(const Rect& region, const cv::Mat& templ, float threshold = 0.7,
                        const cv::Mat& mask = cv::Mat());
    bool match_template(const cv::Mat& source, const cv::Mat& templ,
                        float threshold = 0.7, const cv::Mat& mask = cv::Mat());

    cv::Mat get_mask(const cv::Mat& image, const Color& color, float variance);
    cv::Mat get_mask(const Rect& region, const Color& color, float variance);

    void get_handle(int timeout = 60, bool verbose = false);
    cv::Mat screenshot(const Rect& region = Rect(0, 0, 0, 0), HWND window = hWnd);
    
    RECT get_window_rect();

    void set_tesseract_image(const cv::Mat& image);

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
        return os << std::format("Rect(x={}, y={}, width={}, height={})", r.x, r.y,
                                 r.width, r.height);
    }

    bool set_foreground();
    bool set_foreground_but_hidden();
    bool has_crashed_popup();

    void set_mouse_pos(const Point&);
    void set_mouse_pos(int x, int y);
    void click_at(const Point&, controls::MouseButton,
                  std::chrono::milliseconds delay = std::chrono::milliseconds(50));

    void down(const settings::ActionMapping&,
              std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void up(const settings::ActionMapping&,
            std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void press(const settings::ActionMapping&, bool catch_cursor = false,
               std::chrono::milliseconds delay = std::chrono::milliseconds(50));

    void down(const std::string& key,
              std::chrono::milliseconds delay = std::chrono::milliseconds(0));
    void up(const std::string& key,
            std::chrono::milliseconds delay = std::chrono::milliseconds(0));
    void press(const std::string& key, bool catch_cursor = false,
               std::chrono::milliseconds delay = std::chrono::milliseconds(0));

    void post_down(const settings::ActionMapping&,
                   std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void post_up(const settings::ActionMapping&,
                 std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void post_press(const settings::ActionMapping&, bool catch_cursor = false,
                    std::chrono::milliseconds delay = std::chrono::milliseconds(50));

    void post_key_down(const std::string& key,
                       std::chrono::milliseconds delay = std::chrono::milliseconds(0));
    void post_key_up(const std::string& key,
                     std::chrono::milliseconds delay = std::chrono::milliseconds(0));
    void post_key_press(const std::string& key, bool catch_cursor = false,
                        std::chrono::milliseconds delay = std::chrono::milliseconds(0));

    void post_char(char c);

    void post_mouse_down(controls::MouseButton,
                         std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void post_mouse_up(controls::MouseButton,
                       std::chrono::milliseconds delay = std::chrono::milliseconds(10));
    void post_mouse_press(controls::MouseButton, bool catch_cursor = false,
                          std::chrono::milliseconds delay =
                              std::chrono::milliseconds(100));
    void post_mouse_press_at(const Point&, controls::MouseButton);

    void reset_cursor(POINT& previousPosition);
    
    void post_close();
}
