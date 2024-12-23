#define WIN32_LEAN_AND_MEAN
#include "asapp/util/util.h"
#include <string>
#include <Windows.h>
#include "asapp/core/state.h"

namespace util
{
    bool await(const std::function<bool()>& condition, std::chrono::milliseconds timeout)
    {
        auto start_time = std::chrono::steady_clock::now();
        while (!condition()) {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(
                current_time - start_time);

            if (elapsed_time >= timeout) { return false; }
            asa::core::sleep_for(std::chrono::milliseconds(5));
        }
        return true;
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

    std::string fix(const std::string& src, const std::map<std::string, std::string>& fixes)
    {
        std::string out = src;
        for (const auto& [wrong, right] : fixes) {
            std::size_t index;
            while ((index = out.find(wrong)) != std::string::npos) {
                out.replace(index, wrong.length(), right);
            }
        }
        return out;
    }


}
