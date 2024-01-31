#include "asapp/interfaces/tribemanager.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"
#include "asapp/util/util.h"
#include "asapp/core/state.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <regex>
#include <map>
#include <algorithm>

namespace asa::interfaces
{
    using components::TribeLogMessage;


    namespace
    {
        const char* CONTENT_OCR_WHITELIST = R"(ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789()[]{}-!?/\|+=>*'": )";
        const char* TIMESTAMP_OCR_WHITELIST = R"("Day:0123456789, )";

        const std::map<std::string, std::string> OCR_FIXES = {
                {"\"\n", "- "},
                {"\n", " "},
                {"‘", "'"},
                {"’", "'"},
                {"\"", "'"},
                {"}", ")"},
                {"{", "("},
                {"- Lvb", "- Lvl"},
                {"- iLvl", "- Lvl"},
                {"- Lv¥i", "- Lvl"},
                {"- L¥l", "- Lvl"},
                {"- Lvi", "- Lvl"},
                {"- LvI", "- Lvl"},
                {" - Lv ", " - Lvl "},
                {"- Lyl", "- Lvl"},
                {"- vl ", "- Lvl "},
                {"- Lyi", "- Lvl "},
                {"Lvl1", "Lvl"},
                {"kilied", "killed"},
                {"kilted", "killed"},
                {"kilfed", "killed"},
                {"deatht", "death!"},
                {"'t", "'!"},
                {"  ", " "},
                {"(Pin Coded)!", "(Pin Coded)'"},
        };

        // Multiple events share the same color, but this gives us a basic idea, to
        // get the actual event the contents of the parsed message has to be looked at.
        using Type = TribeLogMessage::Event::EventType;
        const std::map<TribeLogMessage::Event::EventType, window::Color> EVENT_COLORS = {
                {Type::DEMOLISHED, {230, 233, 8}},
                {Type::TRIBE_DESTROYED, {255, 0, 0}},
                {Type::ENEMY_DESTROYED, {252, 223, 148}},
                {Type::DINO_TAMED, {0, 255, 0}},
                {Type::DEMOTED, {236, 160, 5}},
                {Type::TRIBE_GROUP_UPDATED, {228, 232, 235}},
                {Type::DINO_STARVED, {192, 192, 192}},
                {Type::ENEMY_DINO_KILLED, {255, 0, 255}},
        };


        std::string translate(const std::string& src)
        {
            std::string out = src;

            for (const auto& [wrong, right]: OCR_FIXES) {
                std::size_t index;
                while ((index = out.find(wrong)) != std::string::npos) {
                    out.replace(index, wrong.length(), right);
                }
            }
            return out;
        }
    };

    bool TribeManager::is_open() const
    {
        return match_template(tribe_manager_button.area,
                              resources::interfaces::tribemanager);
    }

    void TribeManager::open()
    {
        const auto start = std::chrono::system_clock::now();
        while (!is_open()) {
            window::press(settings::show_tribe_manager, true);
            if (util::await([this]() { return is_open(); }, std::chrono::seconds(5))) {
                break;
            }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotOpenedError(this);
            }
        }
    }

    void TribeManager::close()
    {
        const auto start = std::chrono::system_clock::now();
        while (is_open()) {
            window::press("esc", true);
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }

            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotClosedError(this);
            }
        }
    }

    void TribeManager::update_tribelogs(TribeManager::LogUpdateCallback on_finish)
    {
        open();
        core::sleep_for(std::chrono::seconds(2)); // wait a little to receive updates
        const cv::Mat logs = get_current_logs_image();

        std::thread([this, on_finish, logs]() -> void {
            std::vector<window::Rect> entries = collect_entries(logs);

            LogEntries new_messages;
            bool is_first_logs = tribelog_.empty();

            for (const auto& entry: entries) {
                TribeLogMessage msg = parse(cv::Mat(logs, entry.to_cv()));

                if (is_new_message(msg)) {
                    if (!is_first_logs) { new_messages.push_back(msg); }
                    add_message(msg);
                }
            }
            on_finish(tribelog_, new_messages);
        }).detach();

        close();
    }

    std::vector<window::Rect> TribeManager::collect_entries(const cv::Mat& src) const
    {
        // we only care about the first 30 pixels on the x-axis
        const cv::Mat roi(src, cv::Rect(0, 0, 40, src.rows));
        auto matches = window::locate_all_template(roi, resources::interfaces::day_log);

        // sort the matches by their y-position in descending order
        std::ranges::sort(matches, [](const auto& a, const auto& b)
                -> bool { return a.y > b.y; });

        std::vector<window::Rect> results;
        // Notice that we are skipping the first occurrence because in most cases
        // it is cut off and unreadable anyways.
        for (int i = 1; i < matches.size(); i++) {
            const int x = std::max(0, matches[i].x) - 1;
            const int y = std::max(0, matches[i].y) - 1;
            const int width = 350;
            const int height = (matches[i - 1].y - matches[i].y) - 2;

            results.emplace_back(x, y, width, height);
        }
        return results;
    }

    cv::Mat TribeManager::get_current_logs_image() const
    {
        return window::screenshot(tribe_log_area);
    }

    bool TribeManager::is_new_message(const components::TribeLogMessage& msg) const
    {
        return tribelog_.empty() || tribelog_.front().timestamp >= msg.timestamp;
    }

    void TribeManager::add_message(const components::TribeLogMessage& msg)
    {
        tribelog_.insert(tribelog_.begin(), msg);

        while (tribelog_.size() > 50) { tribelog_.pop_back(); }
    }

    TribeLogMessage TribeManager::parse(const cv::Mat& src)
    {
        static constexpr window::Color timestamp_color{192, 192, 192};
        TribeLogMessage data;
        data.raw_image = src;

        // figure out where the timestamp is to OCR that first and ignore it
        // for the contents.
        cv::Mat timestamp_img = crop_timestamp(src);
        cv::Mat timestamp_mask = window::get_mask(timestamp_img, timestamp_color, 120);

        window::set_tesseract_image(timestamp_mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
        window::tessEngine->SetVariable("tessedit_char_whitelist",
                                        TIMESTAMP_OCR_WHITELIST);

        std::string timestamp_result = window::tessEngine->GetUTF8Text();
        data.timestamp = TribeLogMessage::Timestamp::parse(translate(timestamp_result));

        cv::Mat content;
        src.copyTo(content);

        cv::Mat to_black_out(content, {0, 0, timestamp_img.cols, timestamp_img.rows});
        to_black_out.setTo(cv::Scalar(0, 0, 0));

        data.event = get_message_event(content);

        if (!data.event) {
            std::cerr << "[!] Couldnt parse message on day " << data.timestamp.to_string()
                      << std::endl;
            return data;
        }

        cv::Mat mask = window::get_mask(content, EVENT_COLORS.at(data.event), 120);

        window::set_tesseract_image(mask);
        window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        window::tessEngine->SetVariable("tessedit_char_whitelist", CONTENT_OCR_WHITELIST);

        std::string result = window::tessEngine->GetUTF8Text();

        data.raw_text = result;
        data.content = translate(result);
        return data;
    }

    TribeLogMessage::Event::EventType TribeManager::get_message_event(
            const cv::Mat& src) const
    {
        for (const auto& [type, color]: EVENT_COLORS) {
            cv::Mat mask = window::get_mask(src, color, 100);
            if (cv::countNonZero(mask) > 600) { return type; }
        }
        return components::TribeLogMessage::Event::UNKNOWN;
    }

    cv::Mat TribeManager::crop_timestamp(const cv::Mat& src)
    {
        if (!timestamp_width) {

            cv::Mat comma_roi(src, {40, 13, 50, 3});
            cv::Mat mask = window::get_mask(comma_roi, window::Color(186, 187, 189), 30);
            int pix_at = 0;
            for (int i = 0; i < mask.rows; ++i) {
                for (int j = 0; j < mask.cols; ++j) {
                    if (mask.at<uchar>(i, j) != 0) {
                        pix_at = j;
                        break;
                    }
                }
                if (pix_at) { break; }
            }
            timestamp_width = 36 + pix_at + 85;
        }

        return {src, {0, 0, timestamp_width, 17}};
    }


}