#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "asa/game/controls.h"
#include <algorithm>
#include "asa/core/state.h"

namespace asa::controls
{
    namespace
    {
        constexpr float PIXELS_PER_DEGREE_LR = 129.f / 90.f;
        constexpr float PIXELS_PER_DEGREE_UD = 115.f / 90.f;

        constexpr float MAX_LR_SENS = 3.2f;
        constexpr float MAX_UD_SENS = 3.2f;
        constexpr float MAX_FOV = 1.25f;

        const keyboard_mapping_t static_keymap = {
            {"tab", VK_TAB}, {"f1", VK_F1}, {"f2", VK_F2}, {"f3", VK_F3}, {"f4", VK_F4},
            {"f5", VK_F5}, {"f6", VK_F6}, {"f7", VK_F7}, {"f8", VK_F8}, {"f9", VK_F9},
            {"f10", VK_F10}, {"delete", VK_DELETE}, {"home", VK_HOME}, {"end", VK_END},
            {"backspace", VK_BACK}, {"enter", VK_RETURN}, {"period", VK_OEM_PERIOD},
            {"numpadzero", VK_NUMPAD0}, {"numpadone", VK_NUMPAD1},
            {"numpadtwo", VK_NUMPAD2}, {"numpadthree", VK_NUMPAD3},
            {"numpadfour", VK_NUMPAD4}, {"numpadfive", VK_NUMPAD5},
            {"numpadsix", VK_NUMPAD6}, {"numpadseven", VK_NUMPAD7},
            {"numpadeight", VK_NUMPAD8}, {"NumPadnine", VK_NUMPAD9}, {"ctrl", VK_CONTROL},
            {"esc", VK_ESCAPE}, {"space", VK_SPACE}, {"spacebar", VK_SPACE},
            {"zero", 0x30}, {"one", 0x31}, {"two", 0x32}, {"three", 0x33}, {"four", 0x34},
            {"five", 0x35}, {"six", 0x36}, {"seven", 0x37}, {"eight", 0x38},
            {"nine", 0x39},
            {"backspace", VK_BACK}, {"leftshift", VK_LSHIFT}, {"tilde", VK_OEM_3},
            {"comma", VK_OEM_COMMA}
        };

        keyboard_mapping_t get_keyboard_mapping()
        {
            keyboard_mapping_t mapping = static_keymap;

            for (int i = 32; i < 128; i++) {
                const char c = static_cast<char>(i);
                mapping[std::string(1, c)] = VkKeyScanA(c);
            }
            return mapping;
        }

        const keyboard_mapping_t mapping = get_keyboard_mapping();

        int constexpr get_mouse_flag(const MouseButton button, const bool down)
        {
            switch (button) {
                case LEFT: return down ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
                case RIGHT: return down ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
                case MIDDLE: return down ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
                case MOUSE4:
                case MOUSE5: return down ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
            }
            return -1;
        }

        float get_left_right_factor()
        {
            return MAX_LR_SENS / get_user_setting<float>("LookLeftRightSensitivity");
        }

        float get_up_down_factor()
        {
            return MAX_UD_SENS / get_user_setting<float>("LookUpDownSensitivity");
        }

        float get_fov_factor()
        {
            return MAX_FOV / get_user_setting<float>("FOVMultiplier");
        }
    }

    int get_virtual_keycode(std::string key)
    {
        // dirty fix for now
        if (key == "Equals") { key = "="; } else if (key == "Backslash") { key = "\\"; }

        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return mapping.at(key);
    }

    bool is_mouse_input(const action_mapping& input)
    {
        return input.key.find("Mouse") != std::string::npos;
    }

    bool is_key_input(const action_mapping& input)
    {
        return !is_mouse_input(input);
    }

    void down(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        is_mouse_input(input)
            ? mouse_down(str_to_button.at(input.key), delay)
            : key_down(input.key, delay);
    }

    void release(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        is_mouse_input(input)
            ? mouse_up(str_to_button.at(input.key), delay)
            : key_up(input.key, delay);
    }

    void press(const action_mapping& input, const std::chrono::milliseconds delay)
    {
        is_mouse_input(input)
            ? mouse_press(str_to_button.at(input.key), delay)
            : key_press(input.key, delay);
    }

    void mouse_down(const MouseButton button, const std::chrono::milliseconds delay)
    {
        INPUT input{0};
        input.type = INPUT_MOUSE;

        input.mi.dwFlags = get_mouse_flag(button, true);

        if (button == MOUSE4) { input.mi.mouseData = XBUTTON1; } else if (
            button == MOUSE5) { input.mi.mouseData = XBUTTON2; }

        SendInput(1, &input, sizeof(INPUT));
        core::sleep_for(delay);
    }

    void mouse_up(const MouseButton button, const std::chrono::milliseconds delay)
    {
        INPUT input{0};
        input.type = INPUT_MOUSE;

        input.mi.dwFlags = get_mouse_flag(button, false);

        if (button == MOUSE4) { input.mi.mouseData = XBUTTON1; } else if (
            button == MOUSE5) { input.mi.mouseData = XBUTTON2; }

        SendInput(1, &input, sizeof(INPUT));
        core::sleep_for(delay);
    }

    void mouse_press(MouseButton button, std::chrono::milliseconds delay)
    {
        mouse_down(button);
        core::sleep_for(delay);
        mouse_up(button);
    }

    void mouse_combination_press(MouseButton button, std::string key)
    {
        key_down(key);
        core::sleep_for(std::chrono::milliseconds(20));
        mouse_press(button);
        core::sleep_for(std::chrono::milliseconds(20));
        key_up(key);
    }

    void turn_degrees_lr(int x, int y)
    {
        turn_position(x * PIXELS_PER_DEGREE_LR, y * PIXELS_PER_DEGREE_LR);
    }

    void turn_degrees_ud(int x, int y)
    {
        turn_position(x * PIXELS_PER_DEGREE_UD, y * PIXELS_PER_DEGREE_UD);
    }

    void turn_position(int x, int y)
    {
        INPUT input{0};
        input.type = INPUT_MOUSE;

        input.mi.dx = x * get_left_right_factor() * get_fov_factor();
        input.mi.dy = y * get_up_down_factor() * get_fov_factor();
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

    void key_down(std::string key, std::chrono::milliseconds delay)
    {
        INPUT input{0};
        input.type = INPUT_KEYBOARD;
        int wVk = get_virtual_keycode(key);
        input.ki.wVk = wVk;
        if (HIBYTE(wVk) & 0x01) {
            INPUT shiftInput{0};
            shiftInput.type = INPUT_KEYBOARD;
            shiftInput.ki.wVk = VK_SHIFT;
            SendInput(1, &shiftInput, sizeof(INPUT));
        }

        SendInput(1, &input, sizeof(INPUT));
        core::sleep_for(delay);
    }

    void key_up(std::string key, std::chrono::milliseconds delay)
    {
        INPUT input{0};
        input.type = INPUT_KEYBOARD;
        int wVk = get_virtual_keycode(key);
        input.ki.wVk = wVk;
        input.ki.dwFlags = KEYEVENTF_KEYUP;

        if (HIBYTE(wVk) & 0x01) {
            INPUT shiftInput{0};
            shiftInput.type = INPUT_KEYBOARD;
            shiftInput.ki.wVk = VK_SHIFT;
            shiftInput.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &shiftInput, sizeof(INPUT));
        }

        SendInput(1, &input, sizeof(INPUT));
        core::sleep_for(delay);
    }

    void key_press(std::string key, std::chrono::milliseconds delay)
    {
        key_down(key);
        core::sleep_for(delay);
        key_up(key);
    }

    void key_combination_press(std::string holdKey, std::string pressKey)
    {
        key_down(holdKey, std::chrono::milliseconds(20));
        key_press(pressKey, std::chrono::milliseconds(20));
        key_up(holdKey);
    }
}
