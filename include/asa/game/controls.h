#pragma once
#include "settings.h"

#include <chrono>
#include <string>
#include <unordered_map>

using namespace std::chrono_literals;

namespace asa::controls
{
    using keyboard_mapping_t = std::unordered_map<std::string, int>;

    enum MouseButton
    {
        LEFT,
        RIGHT,
        MIDDLE,
        MOUSE4,
        MOUSE5,
    };

    inline const auto str_to_button = std::unordered_map<std::string, MouseButton>{
        {"LeftMouseButton", LEFT}, {"RightMouseButton", RIGHT},
        {"MiddleMouseButton", MIDDLE}, {"ThumbMouseButton", MOUSE4},
        {"ThumbMouseButton2", MOUSE5},
    };

    int get_virtual_keycode(std::string key);
    bool is_mouse_input(const action_mapping& input);
    bool is_key_input(const action_mapping& input);

    void down(const action_mapping&, std::chrono::milliseconds delay = 10ms);
    void release(const action_mapping&, std::chrono::milliseconds delay = 10ms);
    void press(const action_mapping&, std::chrono::milliseconds delay = 20ms);

    void mouse_down(MouseButton, std::chrono::milliseconds delay = 10ms);
    void mouse_up(MouseButton, std::chrono::milliseconds delay = 10ms);
    void mouse_press(MouseButton, std::chrono::milliseconds delay = 20ms);

    void mouse_combination_press(MouseButton, std::string key);

    void turn_degrees_lr(int x, int y);
    void turn_degrees_ud(int x, int y);

    void turn_position(int x, int y);
    void turn_to(int x, int y);

    void key_down(std::string key, std::chrono::milliseconds delay = 10ms);
    void key_up(std::string key, std::chrono::milliseconds delay = 10ms);
    void key_press(std::string key, std::chrono::milliseconds delay = 20ms);
    void key_combination_press(std::string holdKey, std::string pressKey);
}
