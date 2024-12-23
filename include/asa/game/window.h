#pragma once
#define WIN32_LEAN_AND_MEAN
#include "controls.h"

#include <optional>
#include <string>
#include <vector>
#include <Windows.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/publictypes.h>

namespace asa::window
{
    /**
     * @brief Attempts to initialize tesseract if its not already initialized.
     */
    void tesseract_init();

    void find(std::chrono::seconds timeout = 60s);

    bool set_foreground();

    RECT get_boundaries();

    bool has_crash_popup();

    /**
     * @brief Captures a screenshot of the window using the window handle and windows
     * PrintWindow function so that anything on top of the window will not interfere.
     *
     * @param region A cv::Rect containing the area of the window to screenshot.
     * @param direct_capture Whether to capture the window based on the hwnd.
     * @return A cv::Mat containing the screenshot of the window.
     */
    [[nodiscard]] cv::Mat screenshot(const cv::Rect& region = {0, 0, 1920, 1080},
                                     bool direct_capture = true);

    /**
     * @brief Gets the pixel color at the given coordinate using the window handle so that
     * anything on top of the window will not interefere.
     *
     * @param point The cv::Point containing x- and y-position of the pixel in question.
     *
     * @return A cv::Scalar containing the result in RGB format.
     */
    [[nodiscard]] cv::Vec3b pixel(const cv::Point& point);

    /**
     * @brief Set focus on the window, must have called get_hwnd before.
     */
    void set_focus();

    /**
     * @brief Sends a WM_POST message to the window to close it.
     */
    void close();

    /**
     * @brief Locates a template within a given image.
     *
     * @param _template The template to match, must already be loaded into memory.
     * @param source The source image to find the template in.
     * @param threshold The minimum confidence for a match to be considered.
     * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
     * @param top  [OPTIONAL] A pointer to a float to store the best match in.
     * @param mode [OPTIONAL] The mode used to template match, default TM_CCOEFF_NORMED.
     *
     * @return An std::optional containing a cv::Rect of where the template matched.
     */
    [[nodiscard]] std::optional<cv::Rect> locate(const cv::Mat& _template,
                                                 const cv::Mat& source,
                                                 float threshold = 0.7,
                                                 bool grayscale = false,
                                                 const cv::Mat& mask = {},
                                                 float* top = nullptr,
                                                 int mode = cv::TM_CCOEFF_NORMED);

    /**
    * @brief Locates a template within a given region on the screen.
    *
    * @param _template The template to match, must already be loaded into memory.
    * @param region The region of the screen to match the template in.
    * @param threshold The minimum confidence for a match to be considered.
    * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
    * @param top  [OPTIONAL] A pointer to a float to store the best match in.
    * @param mode [OPTIONAL] The mode used to template match, default TM_CCOEFF_NORMED.
    *
    * @return An std::optional containing a cv::Rect of where the template matched.
    */
    [[nodiscard]] std::optional<cv::Rect> locate(const cv::Mat& _template,
                                                 const cv::Rect& region,
                                                 float threshold = 0.7,
                                                 bool grayscale = false,
                                                 const cv::Mat& mask = {},
                                                 float* top = nullptr,
                                                 int mode = cv::TM_CCOEFF_NORMED);

    /**
    * @brief Locates ALL matches of a template within a given image.
    *
    * @param _template The template to match, must already be loaded into memory.
    * @param source The source image to find the template in.
    * @param threshold The minimum confidence for a match to be considered.
    * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
    *
    * @return A vector consisting of cv::Rect's of the matches found.
    */
    [[nodiscard]] std::vector<cv::Rect> locate_all(const cv::Mat& _template,
                                                   const cv::Mat& source,
                                                   float threshold = 0.7,
                                                   bool grayscale = false,
                                                   const cv::Mat& mask = {});

    /**
    * @brief Locates ALL matches of a template within a given region on the screen.
    *
    * @param _template The template to match, must already be loaded into memory.
    * @param region The region of the screen to match the template in.
    * @param threshold The minimum confidence for a match to be considered.
    * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
    *
    * @return A vector consisting of cv::Rect's of the matches found.
    */
    [[nodiscard]] std::vector<cv::Rect> locate_all(const cv::Mat& _template,
                                                   const cv::Rect& region,
                                                   float threshold = 0.7,
                                                   bool grayscale = false,
                                                   const cv::Mat& mask = {});

    /**
    * @brief Thin wrapper of window::locate to return a boolean instead of std::optional.
    *
    * @param _template The template to match, must already be loaded into memory.
    * @param source The source image to find the template in.
    * @param threshold The minimum confidence for a match to be considered.
    * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
    *
    * @return True if a match was found, false otherwise.
    */
    [[nodiscard]] bool match(const cv::Mat& _template, const cv::Mat& source,
                             float threshold = 0.7, bool grayscale = false,
                             const cv::Mat& mask = {});

    /**
    * @brief Thin wrapper of window::locate to return a boolean instead of std::optional.
    *
    * @param _template The template to match, must already be loaded into memory.
    * @param region The region of the screen to match the template in.
    * @param threshold The minimum confidence for a match to be considered.
    * @param mask [OPTIONAL] A mask to exclude an area of the template in the match.
    *
    * @return True if a match was found, false otherwise.
    */
    [[nodiscard]] bool match(const cv::Mat& _template, const cv::Rect& region,
                             float threshold = 0.7, bool grayscale = false,
                             const cv::Mat& mask = {});

    std::string ocr_threadsafe(const cv::Mat& src, tesseract::PageSegMode mode,
                               const char* whitelist);

    void set_mouse_pos(const cv::Point&);

    void click_at(const cv::Point&, controls::MouseButton,
                  std::chrono::milliseconds delay = 5ms);

    void down(const action_mapping&, std::chrono::milliseconds delay = 1ms);

    void up(const action_mapping&, std::chrono::milliseconds delay = 1ms);

    void press(const action_mapping&, std::chrono::milliseconds delay = 5ms);

    void down(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void up(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void press(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void post_down(const action_mapping&, std::chrono::milliseconds delay = 1ms);

    void post_up(const action_mapping&, std::chrono::milliseconds delay = 10ms);

    void post_press(const action_mapping&, std::chrono::milliseconds delay = 50ms);

    void post_key_down(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void post_key_up(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void post_key_press(const std::string& key, std::chrono::milliseconds delay = 0ms);

    void post_char(char c);

    void post_mouse_down(controls::MouseButton, std::chrono::milliseconds delay = 10ms);

    void post_mouse_up(controls::MouseButton, std::chrono::milliseconds delay = 10ms);

    void post_mouse_press(controls::MouseButton, std::chrono::milliseconds delay = 100ms);

    void post_mouse_press_at(const cv::Point&, controls::MouseButton);
}
