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
    using EventType = TribeLogMessage::EventType;

    namespace
    {
        const char* CONTENT_OCR_WHITELIST =
            R"(ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789()[]{}-!?/\|+=>*'": )";
        const char* TIMESTAMP_OCR_WHITELIST = R"("Day:0123456789, )";

        const std::map<std::string, std::string> OCR_FIXES = {
            {"\"\n", "- "}, {"\n", " "}, {"\"", "'"}, {"}", ")"}, {"{", "("},
            {"- Lvb", "- Lvl"}, {"- iLvl", "- Lvl"}, {"- Lvi", "- Lvl"},
            {"- LvI", "- Lvl"}, {" - Lv ", " - Lvl "}, {"- Lyl", "- Lvl"},
            {"- vl ", "- Lvl "}, {"- Lyi", "- Lvl "}, {"Lvl1", "Lvl"},
            {"kilied", "killed"}, {"kilted", "killed"}, {"kilfed", "killed"},
            {"deatht", "death!"}, {"'t", "'!"}, {"'l", "'!"}, {"  ", " "},
            {"(Pin Coded)!", "(Pin Coded)'"}, {"y'!", ")'!"}, {"Bedy", "Bed)"},
            {"Wallt", "Wall'!"}, {"!!", "!"}, {"((", "("}, {"))", ")"},
            {"Turret!", "Turret'!"}, {"(Pin Coded) ", "(Pin Coded)'"}
        };

        // Multiple events share the same color, but this gives us a basic idea, to
        // get the actual event the contents of the parsed message has to be looked at.
        const std::map<EventType, window::Color> EVENT_COLORS = {
            {EventType::ENEMY_DINO_KILLED, {255, 0, 255}},
            {EventType::DEMOLISHED, {230, 233, 8}},
            {EventType::TRIBE_DESTROYED, {255, 0, 0}},
            {EventType::ENEMY_DESTROYED, {252, 223, 148}},
            {EventType::DINO_TAMED, {0, 255, 0}}, {EventType::DEMOTED, {251, 168, 1}},
            {EventType::PROMOTED, {0, 0, 255}},
            {EventType::TRIBE_GROUP_UPDATED, {255, 255, 255}},
            {EventType::DINO_STARVED, {192, 192, 192}},
            {EventType::PLAYER_ADDED, {0, 252, 252}},
            {EventType::UPLOADED, {237, 108, 137}},
            {EventType::DOWNLOADED, {176, 234, 194}},
        };

        const std::vector<EventType> EVENT_ORDER{
            EventType::ENEMY_DINO_KILLED, EventType::DEMOLISHED,
            EventType::TRIBE_DESTROYED, EventType::ENEMY_DESTROYED, EventType::DINO_TAMED,
            EventType::DINO_STARVED, EventType::DEMOTED, EventType::TRIBE_GROUP_UPDATED,
            EventType::PROMOTED, EventType::PLAYER_ADDED, EventType::UPLOADED,
            EventType::DOWNLOADED
        };


        /**
         * @brief Returns a string with all common OCR mistakes in a src string fixed.
         *
         * @param src The source string to find and fix the mistakes in
         */
        std::string fix(const std::string& src)
        {
            std::string out = src;
            for (const auto& [wrong, right] : OCR_FIXES) {
                std::size_t index;
                while ((index = out.find(wrong)) != std::string::npos) {
                    out.replace(index, wrong.length(), right);
                }
            }
            return out;
        }

        TribeLogMessage::Timestamp parse_timestamp(const cv::Mat& src)
        {
            static constexpr window::Color time_rgb{192, 192, 192};

            const cv::Mat mask = window::get_mask(src, time_rgb, 120);

            window::set_tesseract_image(mask);
            window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
            window::tessEngine->SetVariable("tessedit_char_whitelist",
                                            TIMESTAMP_OCR_WHITELIST);
            const std::string raw = window::tessEngine->GetUTF8Text();
            return TribeLogMessage::Timestamp::parse(fix(raw));
        }

        std::string parse_content(const cv::Mat& src, const EventType event)
        {
            static constexpr window::Color turret_name_color{192, 192, 192};

            const cv::Mat mask = window::get_mask(src, EVENT_COLORS.at(event), 130);

            // For enemy dinos or players killed, the chance of a turret name exists
            if (event == EventType::ENEMY_DINO_KILLED || event ==
                EventType::ENEMY_PLAYER_KILLED) {
                mask |= window::get_mask(src, turret_name_color, 120);
            }

            window::set_tesseract_image(mask);
            window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
            window::tessEngine->SetVariable("tessedit_char_whitelist",
                                            CONTENT_OCR_WHITELIST);

            return window::tessEngine->GetUTF8Text();
        }
    }

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
            close_button_.press();
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotClosedError(this);
            }
        }
    }

    void TribeManager::update_tribelogs(const LogUpdateCallback& on_finish,
                                        const std::chrono::seconds receive_for)
    {
        static constexpr std::chrono::milliseconds min_recv_time{500};
        const bool was_open = is_open();

        open();
        core::sleep_for(receive_for.count() ? receive_for : min_recv_time);
        const cv::Mat logs = get_current_logs_image();

        std::thread([this, on_finish, logs]() -> void {
            // If this is the first time we check we shouldnt be pushing any messages
            // as new events as we have no previous data to base it off.
            // It also means we should validate that the day of our first message
            // somewhat matches the current time determined via the HUD.
            const bool is_initial_check = tribelog_.empty();
            // Keep track of whether we already added a new message, if we did
            // allow messages to be added that have the exact same timestamp as
            // our most recent addition.
            bool any_new = false;

            const std::vector<window::Rect> entries = collect_entries(logs);
            LogEntries new_;

            for (const auto& entry : entries) {
                const TribeLogMessage msg = parse(cv::Mat(logs, entry.to_cv()));
                if (is_new_message(msg.timestamp, any_new)) {
                    if (!is_initial_check) { new_.insert(new_.begin(), msg); }
                    add_message(msg);
                    any_new = true;
                }
            }
            on_finish(tribelog_, new_);
        }).detach();
        if (!was_open) { close(); }
    }

    std::vector<window::Rect> TribeManager::collect_entries(const cv::Mat& src) const
    {
        // we only care about the first 30 pixels on the x-axis
        const cv::Mat roi(src, cv::Rect(0, 0, 40, src.rows));
        auto matches = window::locate_all_template(roi, resources::interfaces::day_log,
                                                   0.85);

        // sort the matches by their y-position in descending order
        std::ranges::sort(matches, [](const auto& a, const auto& b) -> bool {
            return a.y > b.y;
        });

        std::vector<window::Rect> results;
        // Notice that we are skipping the first occurrence because in most cases
        // it is cut off and unreadable anyways.
        for (int i = 1; i < matches.size(); i++) {
            const int x = std::max(0, matches[i].x - 1);
            const int y = std::max(0, matches[i].y - 1);
            const int height = (matches[i - 1].y - matches[i].y) - 2;

            results.emplace_back(x, y, 350, height);
        }
        return results;
    }

    cv::Mat TribeManager::get_current_logs_image() const
    {
        return window::screenshot(tribe_log_area);
    }

    bool TribeManager::is_new_message(const TribeLogMessage::Timestamp time,
                                      const bool allow_equal) const
    {
        if (tribelog_.empty()) { return true; }

        auto& prev = tribelog_.front().timestamp;

        if (allow_equal) { return prev < time || prev == time; }
        return prev < time;
    }

    void TribeManager::add_message(const components::TribeLogMessage& msg)
    {
        tribelog_.insert(tribelog_.begin(), msg);

        while (tribelog_.size() > 50) { tribelog_.pop_back(); }
    }

    TribeLogMessage TribeManager::parse(const cv::Mat& src)
    {
        TribeLogMessage msg;
        msg.raw_image = src.clone();

        // OCR timestamp seperately so we can use different whitelists.
        cv::Mat timestamp_img = crop_timestamp(src);
        msg.timestamp = parse_timestamp(timestamp_img);
        // We no longer care about the timestamp of this message, black it out.
        timestamp_img.setTo(cv::Scalar(0, 0, 0));

        // If this fails theres no point even trying as we can't properly mask.
        if (msg.event = get_message_event(src); msg.event == EventType::UNKNOWN) {
            std::cerr << "[!] Unknown event: " << msg.timestamp.to_string() << std::endl;
            return msg;
        }

        // Parse and fix the actual content of the message.
        msg.raw_text = parse_content(src, msg.event);
        msg.content = fix(msg.raw_text);

        if (msg.content.empty()) { msg.content = "???"; }
        else { evaluate_message_event(msg); }
        return msg;
    }

    TribeLogMessage::EventType TribeManager::get_message_event(const cv::Mat& src) const
    {
        for (const TribeLogMessage::EventType type : EVENT_ORDER) {
            const cv::Mat mask = window::get_mask(src, EVENT_COLORS.at(type), 40);
            if (cv::countNonZero(mask) > 250) { return type; }
        }
        return TribeLogMessage::EventType::UNKNOWN;
    }

    void TribeManager::evaluate_message_event(TribeLogMessage& msg) const
    {
        if (msg.content.empty()) { throw std::exception("Cant evaluate empty message."); }

        switch (msg.event) {
        // red events, detroyed is distinct, otherwise look for Tribemember.
        case EventType::TRIBE_DESTROYED:
        case EventType::TRIBE_PLAYER_KILLED:
        case EventType::TRIBE_DINO_KILLED:
        {
            if (msg.content.find("destroyed") != std::string::npos) {
                msg.event = EventType::TRIBE_DESTROYED;
            }
            else if (msg.content.find("Tribemember") != std::string::npos) {
                msg.event = EventType::TRIBE_PLAYER_KILLED;
            }
            else { msg.event = EventType::TRIBE_DINO_KILLED; }
            break;
        }
        // white / gray events, all have a distinct identifier.
        case EventType::DINO_STARVED:
        case EventType::TRIBE_GROUP_UPDATED:
        case EventType::DINO_CRYOD:
        {
            if (msg.content.find("starved") != std::string::npos) {
                msg.event = EventType::DINO_STARVED;
            }
            else if (msg.content.find("froze") != std::string::npos) {
                msg.event = EventType::DINO_CRYOD;
            }
            else { msg.event = EventType::TRIBE_GROUP_UPDATED; }
            break;
        }
        // purple events, if a dino is killed the type of dino will be in
        // parantheses as well as the dinos tribe. Will get false matches
        // for out of tribe dinos but there is no reliable way to handle that.
        case EventType::ENEMY_DINO_KILLED:
        case EventType::CLAIMED:
        case EventType::ENEMY_PLAYER_KILLED:
        {
            if (msg.content.find("claimed") != std::string::npos) {
                msg.event = EventType::CLAIMED;
            }
            else if (std::ranges::count(msg.content, '(') >= 2 ||
                std::ranges::count(msg.content, ')') >= 2) {
                msg.event = EventType::ENEMY_DINO_KILLED;
            }
            else { msg.event = EventType::ENEMY_PLAYER_KILLED; }
            break;
        }
        // yellow events, either unclaimed or demolished
        case EventType::UNCLAIMED:
        case EventType::DEMOLISHED:
        case EventType::PLAYER_REMOVED:
        {
            if (msg.content.find("removed") != std::string::npos) {
                msg.event = EventType::PLAYER_REMOVED;
            }
            else if (msg.content.find("demolished") != std::string::npos) {
                msg.event = EventType::DEMOLISHED;
            }
            else { msg.event = EventType::UNCLAIMED; }
            break;
        }
        default:
            // All the events that have a distinct color are already accurate.
            break;
        }
    }

    cv::Mat TribeManager::crop_timestamp(const cv::Mat& src)
    {
        if (!timestamp_width) {
            const cv::Mat comma_roi(src, {40, 13, 50, 3});
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

        return {src, cv::Rect(0, 0, timestamp_width, 17)};
    }
}
