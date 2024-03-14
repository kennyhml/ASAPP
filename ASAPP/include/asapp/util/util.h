#pragma once
#include <chrono>
#include <functional>
#include <map>

#include "opencv2/imgproc.hpp"

namespace util
{
    bool await(const std::function<bool()>& condition, std::chrono::milliseconds timeout);

    template <typename Duration>
    bool timedout(const std::chrono::system_clock::time_point& start, Duration timeout)
    {
        auto now = std::chrono::system_clock::now();
        return now - start >= timeout;
    }


    template <typename Type>
    const char* GetName(const Type& obj) { return typeid(obj).name(); }

    bool is_only_one_bit_set(int bitfield);
    void set_clipboard(const std::string& term);

    cv::Mat mask_alpha_channel(const cv::Mat& src);

    template <typename Cast>
    Cast get_elapsed(std::chrono::system_clock::time_point start)
    {
        return std::chrono::duration_cast<Cast>(std::chrono::system_clock::now() - start);
    }

    bool iequal(const std::string& a, const std::string& b);


    /**
     * @brief Returns a string with all common OCR mistakes in a src string
     * fixed.
     *
     * @param src The source string to find and fix the mistakes in
     */
    std::string fix(const std::string& src,
                    const std::map<std::string, std::string>& fixes);
}
