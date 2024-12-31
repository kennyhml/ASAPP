#pragma once
#include <chrono>
#include <functional>
#include <map>
#include "game/window.h"

namespace asa::utility
{
    bool await(const std::function<bool()>& condition, std::chrono::milliseconds timeout);

    std::chrono::system_clock::time_point from_t(time_t time);

    template<typename Duration>
    bool timedout(const std::chrono::system_clock::time_point& start, Duration timeout)
    {
        const auto now = std::chrono::system_clock::now();
        return now - start >= timeout;
    }

    template<typename Duration>
    bool timedout(const int64_t start, const Duration& max)
    {
        const auto now = std::chrono::system_clock::now();
        return (now - std::chrono::system_clock::from_time_t(start)) > max;
    }

    template<typename Cast>
    Cast get_elapsed(const std::chrono::system_clock::time_point& start)
    {
        return std::chrono::duration_cast<Cast>(std::chrono::system_clock::now() - start);
    }

    struct stopwatch
    {
    public:
        template<typename T = std::chrono::milliseconds>
        [[nodiscard]] T elapsed() const { return get_elapsed<T>(start_); }

        template<typename T>
        [[nodiscard]] bool timedout(const T max) const
        {
            return utility::timedout(start_, max);
        }

    private:
        std::chrono::system_clock::time_point start_ = std::chrono::system_clock::now();
    };

    /**
     * @brief This function helps optimize the template match on all interactions
     * that are underlined by the cyan line which will appear when one or more options
     * are available on a structure.
     *
     * @return The roi to look for the interaction text in if found, else std::nullopt.
     */
    std::optional<cv::Rect> find_multi_interactable_line(
        const cv::Mat& src, bool* match_full = nullptr);


    cv::Rect max_contour(const std::vector<std::vector<cv::Point> >& contours);

    /**
     * @brief Masks the given image using a given color and variance.
     *
     * @param image The image to create the mask of.
     * @param color The color to mask the image for.
     * @param variance The variance a color may have from the given color
     *
     * @return A mask of the given image where each matching pixel is 1, else 0.
     */
    cv::Mat mask(const cv::Mat& image, const cv::Vec3b& color, int variance);

    /**
     * @brief Masks the given image using a given color and variance.
     *
     * @param roi The area of the image to create the mask of.
     * @param color The color to mask the image for.
     * @param variance The variance a color may have from the given color
     *
     * @return A mask of the given image where each matching pixel is 1, else 0.
     */
    cv::Mat mask(const cv::Rect& roi, const cv::Vec3b& color, int variance);

    /**
     * @brief Helper function to create a low and high color range from a color & variance.
     */
    void get_ranges(const cv::Vec3b& src, cv::Vec3b& low, cv::Vec3b& high, int v);

    /**
     * @brief Counts the amount of pixels that match a given color within a given image,
     * allowing for a given variance.
     */
    int count_matches(const cv::Mat& img, const cv::Vec3b& color, int variance);

    /**
     * @brief Counts the amount of pixels that match a given color within a given image,
     * allowing for a given variance.
     */
    int count_matches(const cv::Rect& img, const cv::Vec3b& color, int variance);

    bool pixel_matches(const cv::Vec3b& c1, const cv::Vec3b& c2, int tolerance);

    bool roi_in_bounds(const cv::Rect& roi, const cv::Rect& bounds);

    bool point_in_bounds(const cv::Point& point, const cv::Rect& bounds);

    bool is_only_one_bit_set(int bitfield);

    void set_clipboard(const std::string& term);

    cv::Mat mask_alpha_channel(const cv::Mat& src);

    bool iequal(const std::string& a, const std::string& b);

    cv::Point center_of(const cv::Rect& rect);

    /**
     * @brief Returns a string with all common OCR mistakes in a src string
     * fixed.
     *
     * @param src The source string to find and fix the mistakes in
     */
    std::string fix(const std::string& src,
                    const std::map<std::string, std::string>& fixes);

    void to_lower(std::string& str);
}
