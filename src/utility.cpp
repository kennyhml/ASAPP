#define WIN32_LEAN_AND_MEAN
#include "asa/utility.h"
#include <string>
#include <Windows.h>
#include "asa/core/state.h"
#include "asa/game/window.h"

namespace asa::utility
{
    std::optional<cv::Rect> find_multi_interactable_line(
        const cv::Mat& src, bool* match_full)
    {
        static cv::Vec3b cyan{0, 255, 255};
        const auto img = mask(src, cyan, 10);
        if (match_full) {
            *match_full = cv::countNonZero(img) > 200;
        }

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double longest = 0;
        cv::Rect best_contour;
        for (const auto& contour: contours) {
            const auto rect = cv::boundingRect(contour);
            // not the shape we are looking for.
            if (rect.width < rect.height || rect.width < 150) { continue; }

            if (const auto length = cv::arcLength(contour, true); length > longest) {
                longest = length;
                best_contour = rect;
            }
        }
        return best_contour.empty() ? std::nullopt : std::optional(best_contour);
    }

    cv::Rect max_contour(const std::vector<std::vector<cv::Point> >& contours)
    {
        int max_size = 0;
        cv::Rect max_cont;

        for (const auto& contour: contours) {
            if (contourArea(contour) > max_size) {
                max_size = cv::contourArea(contour);
                max_cont = boundingRect(contour);
            }
        }
        return max_cont;
    }

    cv::Mat mask(const cv::Mat& image, const cv::Vec3b& color, const int variance)
    {
        cv::Vec3b low;
        cv::Vec3b high;
        get_ranges(color, low, high, variance);

        cv::Mat mask;
        cv::inRange(image, low, high, mask);
        return mask;
    }

    cv::Mat mask(const cv::Rect& roi, const cv::Vec3b& color, int variance)
    {
        return mask(screenshot(roi), color, variance);
    }

    void get_ranges(const cv::Vec3b& src, cv::Vec3b& low, cv::Vec3b& high, const int v)
    {
        auto [b, g, r] = src.val;

        low = cv::Vec3b(std::max(0, r - v),
                        std::max(0, g - v),
                        std::max(0, b - v));

        high = cv::Vec3b(std::min(255, r + v),
                         std::min(255, g + v),
                         std::min(255, b + v));
    }

    int count_matches(const cv::Mat& img, const cv::Vec3b& color, const int variance)
    {
        const cv::Mat masked = mask(img, color, variance);
        return cv::countNonZero(masked);
    }

    int count_matches(const cv::Rect& img, const cv::Vec3b& color, int variance)
    {
        return count_matches(screenshot(img), color, variance);
    }

    bool pixel_matches(const cv::Vec3b& c1, const cv::Vec3b& c2, const int tolerance)
    {
        return abs(c1[0] - c2[0]) <= tolerance &&
               abs(c1[1] - c2[1]) <= tolerance &&
               abs(c1[2] - c2[2]) <= tolerance;
    }

    bool roi_in_bounds(const cv::Rect& roi, const cv::Rect& bounds)
    {
        return roi.x > bounds.x && roi.y > bounds.y &&
               roi.x + roi.width < bounds.x + bounds.width &&
               roi.y + roi.height < bounds.y + bounds.height;
    }

    bool point_in_bounds(const cv::Point& point, const cv::Rect& bounds)
    {
        return point.x > bounds.x
               && point.y > bounds.y
               && point.x < bounds.x + bounds.width
               && point.y < bounds.y + bounds.height;
    }


    bool await(const std::function<bool()>& condition, std::chrono::milliseconds timeout)
    {
        auto start_time = std::chrono::steady_clock::now();
        while (!condition()) {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(
                current_time - start_time);

            if (elapsed_time >= timeout) { return false; }
            checked_sleep(5ms);
        }
        return true;
    }

    std::chrono::system_clock::time_point from_t(const time_t time)
    {
        return std::chrono::system_clock::from_time_t(time);
    }

    bool timedout(const std::chrono::system_clock::time_point& start,
                  const std::chrono::milliseconds timeout)
    {
        auto now = std::chrono::system_clock::now();
        auto timePassed = now - start;

        return timePassed >= timeout;
    }

    bool timedout(const std::chrono::system_clock::time_point& start,
                  const std::chrono::seconds timeout)
    {
        return timedout(
            start, std::chrono::duration_cast<std::chrono::milliseconds>(timeout));
    }

    bool is_only_one_bit_set(int bitfield)
    {
        return bitfield != 0 && (bitfield & (bitfield - 1)) == 0;
    }

    void set_clipboard(const std::string& text)
    {
        auto glob = GlobalAlloc(GMEM_FIXED, (text.size() + 1) * sizeof(char));
        memcpy(glob, text.c_str(), text.size() + 1);

        OpenClipboard(nullptr);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, glob);
        CloseClipboard();
    }

    cv::Mat mask_alpha_channel(const cv::Mat& src)
    {
        cv::Mat copy;
        src.copyTo(copy);

        if (copy.channels() != 4) { return cv::Mat::ones(copy.size(), CV_8U); }

        std::vector<cv::Mat> channels;
        cv::split(copy, channels);
        cv::Mat alpha_channel = channels[3];
        cv::Mat mask = (alpha_channel > 0);

        if (cv::countNonZero(mask) == 0) { return cv::Mat::ones(copy.size(), CV_8U); }
        return mask;
    }

    bool iequal(const std::string& a, const std::string& b)
    {
        return std::ranges::equal(a, b, [](const char ca, const char cb) -> bool {
            return std::tolower(static_cast<unsigned char>(ca)) == std::tolower(
                       static_cast<unsigned char>(cb));
        });
    }

    cv::Point center_of(const cv::Rect& rect)
    {
        return {rect.x + rect.width / 2, rect.y + rect.height / 2};
    }

    std::string fix(const std::string& src,
                    const std::map<std::string, std::string>& fixes)
    {
        std::string out = src;
        for (const auto& [wrong, right]: fixes) {
            std::size_t index;
            while ((index = out.find(wrong)) != std::string::npos) {
                out.replace(index, wrong.length(), right);
            }
        }
        return out;
    }

    void to_lower(std::string& str)
    {
        std::ranges::transform(str, str.begin(), [](const unsigned char c) {
            return std::tolower(c);
        });
    }
}
