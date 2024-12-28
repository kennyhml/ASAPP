#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "asa/game/settings.h"
#include "asa/game/embedded.h"

#include <optional>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/publictypes.h>

using namespace std::chrono_literals;

namespace asa
{
    enum class MouseButton
    {
        LEFT,
        RIGHT,
        MIDDLE,
        MOUSE4,
        MOUSE5,
    };

    /**
     * @brief Gets the window handle of the game, assuming that the game is running.
     *
     * @param timeout Time limit to find the window, instant if not provided.
     *
     * @return The acquired window handle, the value is also stored and used internally.
     *
     * @throws window_not_found If the window was not found within the time limit.
     */
    [[maybe_unused]] HWND get_window_handle(
        const std::optional<std::chrono::seconds>& timeout = std::nullopt);

    /**
     * @brief Gets the boundaries of the game window.
     *
     * @throws window_not_found If no window is available.
     */
    [[nodiscard]] cv::Rect get_window_boundaries();

    /**
     * @brief Checks whether a crash popup exists based on their window titles.
     *
     * @return True if a crash popup was found, false otherwise.
     */
    [[nodiscard]] bool has_crash_popup();

    /**
     * @brief Checks if the hwnd previously obtrained through `get_window_handle`
     * is still valid for the game window.
     *
     * @return True if the window handle is still valid, false otherwise.
     */
    [[nodiscard]] bool is_hwnd_valid();

    /**
     * @brief Attempts to initialize tesseract if its not already initialized.
     *
     * Tesseract initialization is required to perform OCR.
     *
     * @throws tesseract_not_initialized If the tesseract engine could not be initialized.
     */
    void initialize_tesseract();

    /**
     * @brief Sets focus on the game window.
     *
     * @throws window_not_found If no window is available.
     */
    void set_window_focus();

    /**
     * @brief Sends a WM_POST message to the window to close it.
     */
    void quit();

    /**
     * @brief Sets the mouse cursor to the provided (x, y) location.
     */
    void set_mouse_pos(const cv::Point&);

    /**
     * @brief Posts a "down" event for the provided action mapping.
     *
     * @throws invalid_action_mapping If the action mapping is unknown or not supported.
     */
    void post_down(const action_mapping&);

    /**
    * @brief Posts a "down" event for the provided mouse button.
    */
    void post_down(MouseButton);

    /**
     * @brief Posts a "down" event for the provided key.
     *
     * @throws invalid_key_input If the key is unknown or not supported.
     */
    void post_down(const std::string& key);

    /**
     * @brief Posts an "up" event for the provided action mapping.
     *
     * @throws invalid_action_mapping If the action mapping is unknown or not supported.
     */
    void post_up(const action_mapping&);

    /**
     * @brief Posts an "up" event for the provided mouse button.
     */
    void post_up(MouseButton);

    /**
     * @brief Posts an "up" event for the provided key.
     *
     * @throws invalid_key_input If the key is unknown or not supported.
     */
    void post_up(const std::string& key);

    /**
     * @brief Posts a press event for the provided action mapping.
     *
     * @param duration The duration (in milliseconds) to keep the button down.
     *
     * @throws invalid_action_mapping If the action mapping is unknown or not supported.
     */
    void post_press(const action_mapping&, std::chrono::milliseconds duration = 25ms);

    /**
     * @brief Posts a press event for the provided mouse button.
     *
     * @param location The location to send the keypress at, nullopt if current.
     * @param duration The duration (in milliseconds) to keep the button down.
     */
    void post_press(MouseButton, const std::optional<cv::Point>& location = std::nullopt,
                    std::chrono::milliseconds duration = 25ms);

    /**
     * @brief Posts a key event for the provided key.
     *
     * @param key The key to press, upper/ lowercase is not respected!
     * @param duration The duration (in milliseconds) to keep the button down.
     *
     * @throws invalid_key_input If the key is unknown or not supported.
     *
     * @remark If case sensitivity is important, use @link post_character \endlink.
     */
    void post_press(const std::string& key, std::chrono::milliseconds duration = 25ms);

    /**
     * @brief Posts a key event with the given character (case sensitive).
     *
     * @param c The character to post.
     */
    void post_character(char c);

    /**
     * @brief Writes the given text to the game window (case sensitive).
     *
     * @param text The text to write to the game.
     */
    void typewrite(const std::string& text);

    /**
     * @brief Posts a key combination press to the window.
     *
     * @param down The key (modifier) to hold down.
     * @param press The key to press.
     *
     * @throws invalid_key_input If the key is unknown or not supported.
     */
    void post_combination(const std::string& down, const std::string& press);

    /**
     * @brief Turns by the provided amounts (in degrees).
     *
     * @param x The amount to turn left / right in degrees.
     * @param y The amount to turn up / down in degrees.
     */
    void turn(int x, int y);

    void turn_to(int x, int y);

    /**
     * @brief Captures a screenshot of the window using the window handle and windows
     * PrintWindow function so that anything on top of the window will not interfere.
     *
     * @param region A cv::Rect containing the area of the window to screenshot.
     * @param direct_capture Whether to capture the window based on the hwnd.
     *
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
    * @brief Thin wrapper of locate to return a boolean instead of std::optional.
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
    * @brief Thin wrapper of locate to return a boolean instead of std::optional.
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

    /**
     * @brief Uses the tesseract engine to extract text from the provided image.
     *
     * @param src The image to extract the text from, preprocessing should be done before.
     * @param mode The page segmentation mode to use for the text extraction.
     * @param whitelist The character whitelist to use.
     *
     * @return The content extracteded from the image, may be fautly!
     *
     * @throws tesseract_not_initialized If the tesseract engine is not available.
     */
    [[nodiscard]] std::string ocr_threadsafe(const cv::Mat& src,
                                             tesseract::PageSegMode mode,
                                             const char* whitelist);
}
