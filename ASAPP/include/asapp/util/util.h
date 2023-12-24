#pragma once
#include <chrono>
#include <functional>
#include "opencv2/imgproc.hpp"

namespace util
{
    bool await(const std::function<bool()>& condition, std::chrono::milliseconds timeout);

    bool timedout(const std::chrono::system_clock::time_point& start,
                  std::chrono::milliseconds timeout);

    bool timedout(const std::chrono::system_clock::time_point& start,
                  std::chrono::seconds timeout);

    template <typename Type>
    const char* GetName(const Type& obj) { return typeid(obj).name(); }

    bool is_only_one_bit_set(int bitfield);
    void set_clipboard(const std::string& term);

    cv::Mat mask_alpha_channel(const cv::Mat& src);

    template <typename Cast>
    Cast get_elapsed(std::chrono::system_clock::time_point start)
    {
        return std::chrono::duration_cast<Cast>(
            std::chrono::system_clock::now() - start);
    }
}
