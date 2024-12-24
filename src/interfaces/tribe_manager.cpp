#include "asa/interfaces/tribe_manager.h"
#include "asa/interfaces/exceptions.h"
#include "asa/utility.h"
#include "asa/core/state.h"
#include "asa/network/queries.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <regex>

namespace asa
{
    using tribelog_message::EventType;

    namespace
    {
        auto CONTENT_OCR_WHITELIST =
                R"(ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789()[]{}-!?/\|+=>*'": )";
        auto TIMESTAMP_OCR_WHITELIST = R"("Day:0123456789, )";

        const std::map<std::string, std::string> OCR_FIXES = {
            {"\"\n", "- "}, {"\n", " "}, {"\"", "'"}, {"}", ")"}, {"{", "("},
            {"- Lvb", "- Lvl"}, {"- iLvl", "- Lvl"}, {"- Lvi", "- Lvl"},
            {"- LvI", "- Lvl"}, {" - Lv ", " - Lvl "}, {"- Lyl", "- Lvl"},
            {"- vl ", "- Lvl "}, {"- Lyi", "- Lvl "}, {"Lvl1", "Lvl"},
            {"kilied", "killed"}, {"kilted", "killed"}, {"kilfed", "killed"},
            {"deatht", "death!"}, {"'t", "'!"}, {"'l", "'!"}, {"  ", " "},
            {"(Pin Coded)!", "(Pin Coded)'"}, {"y'!", ")'!"}, {"Bedy", "Bed)"},
            {"Pin Codedy", "Pin Coded)"}, {"Wallt", "Wall'!"}, {"!!", "!"}, {"((", "("},
            {"))", ")"}, {"''", "'"}, {"Turret!", "Turret'!"},
            {"(Pin Coded) ", "(Pin Coded)'"}
        };

        // Multiple events share the same color, but this gives us a basic idea,
        // to get the actual event the contents of the parsed message has to be
        // looked at.
        const std::map<EventType, cv::Vec3b> EVENT_COLORS = {
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

        const std::vector EVENT_ORDER{
            EventType::ENEMY_DINO_KILLED, EventType::DEMOLISHED,
            EventType::TRIBE_DESTROYED, EventType::ENEMY_DESTROYED, EventType::DINO_TAMED,
            EventType::DINO_STARVED, EventType::DEMOTED, EventType::TRIBE_GROUP_UPDATED,
            EventType::PROMOTED, EventType::PLAYER_ADDED, EventType::UPLOADED,
            EventType::DOWNLOADED
        };


        tribelog_message::timestamp parse_timestamp(const cv::Mat& src)
        {
            static constexpr cv::Vec3b time_rgb{192, 192, 192};

            const cv::Mat mask = utility::mask(src, time_rgb, 120);
            const std::string raw = window::ocr_threadsafe(
                mask, tesseract::PSM_SINGLE_LINE, TIMESTAMP_OCR_WHITELIST);

            return tribelog_message::timestamp::parse(utility::fix(raw, OCR_FIXES));
        }

        std::string parse_content(const cv::Mat& src, const EventType event)
        {
            static constexpr cv::Vec3b turret_name_color{192, 192, 192};

            const cv::Mat mask = utility::mask(src, EVENT_COLORS.at(event), 130);

            // For enemy dinos or players killed, the chance of a turret name
            // exists
            if (event == EventType::ENEMY_DINO_KILLED || event ==
                EventType::ENEMY_PLAYER_KILLED) {
                mask |= utility::mask(src, turret_name_color, 120);
            }

            return window::ocr_threadsafe(mask, tesseract::PSM_SINGLE_BLOCK,
                                          CONTENT_OCR_WHITELIST);
        }

        std::unique_ptr<server> last_server_info = nullptr;

        bool refresh_server_data()
        {
            // nothing to do, data should still be valid.
            if (last_server_info && !utility::timedout(last_server_info->last_queried,
                                                       std::chrono::seconds(300))) {
                return true;
            }

            auto sv = get_server("LastSessionJoinedPerCategory3");
            if (!sv.has_value()) {
                std::cerr << "[!] Tribelogs not checked, server not found." << std::endl;
                last_server_info = nullptr;
                return false;
            }
            last_server_info = std::make_unique<server>(sv.value());
            return true;
        }

        bool is_valid_timestamp(const tribelog_message::timestamp timestamp)
        {
            if (!last_server_info) { return false; }
            return std::abs(last_server_info->day - timestamp.day) < 2;
        }
    }

    bool tribe_manager::is_open() const
    {
        return window::match(embedded::interfaces::tribemanager,
                             tribe_manager_button.area);
    }

    void tribe_manager::open()
    {
        const auto start = std::chrono::system_clock::now();
        while (!is_open()) {
            window::press(get_action_mapping("ShowTribeManager"));
            if (utility::await([this]() { return is_open(); }, 5s)) {
                break;
            }

            if (utility::timedout(start, std::chrono::seconds(30))) {
                throw failed_to_open(this);
            }
        }
    }

    void tribe_manager::close()
    {
        const auto start = std::chrono::system_clock::now();
        while (is_open()) {
            close_button_.press();
            if (utility::await([this]() { return !is_open(); }, 5s)) {
                return;
            }

            // Increased timeout to 60 seconds
            if (utility::timedout(start, 60s)) {
                throw failed_to_close(this);
            }
        }
    }

    void tribe_manager::update_tribelogs(const log_update_callback_t& on_finish,
                                         const std::chrono::seconds receive_for)
    {
        static constexpr std::chrono::milliseconds min_recv_time{500};
        const bool was_open = is_open();

        open();
        checked_sleep(receive_for.count() ? receive_for : min_recv_time);
        const cv::Mat logs = get_current_logs_image();

        std::thread([this, on_finish, logs]() -> void {
            if (!refresh_server_data()) { return; }

            const bool is_initial_check = tribelog_.empty();
            bool any_new = false;

            const std::vector<cv::Rect> entries = collect_entries(logs);
            log_entries_t new_;

            for (const auto& entry: entries) {
                const tribelog_message msg = parse(cv::Mat(logs, entry));
                if (is_new_message(msg.time, any_new) &&
                    is_valid_timestamp(msg.time)) {
                    if (!is_initial_check) { new_.insert(new_.begin(), msg); }
                    add_message(msg);
                    any_new = true;
                }
            }
            on_finish(tribelog_, new_);
        }).detach();
        if (!was_open) { close(); }
    }

    std::vector<cv::Rect> tribe_manager::collect_entries(const cv::Mat& src) const
    {
        // we only care about the first 30 pixels on the x-axis
        const cv::Mat roi(src, cv::Rect(0, 0, 40, src.rows));
        auto matches = window::locate_all(embedded::interfaces::day_log, roi, 0.81);

        // sort the matches by their y-position in descending order
        std::ranges::sort(matches, [](const auto& a, const auto& b) -> bool {
            return a.y > b.y;
        });

        std::vector<cv::Rect> results;
        // Notice that we are skipping the first occurrence because in most
        // cases it is cut off and unreadable anyways.
        for (int i = 1; i < matches.size(); i++) {
            const int x = std::max(0, matches[i].x - 1);
            const int y = std::max(0, matches[i].y - 1);
            const int height = (matches[i - 1].y - matches[i].y) - 2;

            results.emplace_back(x, y, 350, height);
        }
        return results;
    }

    cv::Mat tribe_manager::get_current_logs_image() const
    {
        return window::screenshot(tribe_log_area);
    }

    bool tribe_manager::is_new_message(const tribelog_message::timestamp time,
                                       const bool allow_equal) const
    {
        if (tribelog_.empty()) { return true; }

        auto& prev = tribelog_.front().time;

        if (allow_equal) { return prev < time || prev == time; }
        return prev < time;
    }

    void tribe_manager::add_message(const tribelog_message& msg)
    {
        tribelog_.insert(tribelog_.begin(), msg);

        while (tribelog_.size() > 50) { tribelog_.pop_back(); }
    }

    tribelog_message tribe_manager::parse(const cv::Mat& src)
    {
        tribelog_message msg;
        msg.raw_image = src.clone();

        // OCR timestamp seperately so we can use different whitelists.
        cv::Mat timestamp_img = crop_timestamp(src);

        msg.time = parse_timestamp(timestamp_img);
        // We no longer care about the timestamp of this message, black it out.
        timestamp_img.setTo(cv::Scalar(0, 0, 0));

        // If this fails theres no point even trying as we can't properly mask.
        if (msg.type = get_message_event(src); msg.type == EventType::UNKNOWN) {
            std::cerr << "[!] Unknown event: " << msg.time.to_string() << std::endl;
            return msg;
        }

        // Parse and fix the actual content of the message.
        msg.raw_text = parse_content(src, msg.type);
        msg.content = utility::fix(msg.raw_text, OCR_FIXES);

        if (msg.content.empty()) { msg.content = "???"; } else {
            evaluate_message_event(msg);
        }
        return msg;
    }

    tribelog_message::EventType tribe_manager::get_message_event(const cv::Mat& src) const
    {
        for (const tribelog_message::EventType type: EVENT_ORDER) {
            const cv::Mat mask = utility::mask(src, EVENT_COLORS.at(type), 40);
            if (cv::countNonZero(mask) > 250) { return type; }
        }
        return tribelog_message::EventType::UNKNOWN;
    }

    void tribe_manager::evaluate_message_event(tribelog_message& msg) const
    {
        if (msg.content.empty()) { throw std::exception("Cant evaluate empty message."); }

        switch (msg.type) {
            // red events, detroyed is distinct, otherwise look for Tribemember.
            case EventType::TRIBE_DESTROYED:
            case EventType::TRIBE_PLAYER_KILLED:
            case EventType::TRIBE_DINO_KILLED: {
                if (msg.content.find("destroyed") != std::string::npos) {
                    msg.type = EventType::TRIBE_DESTROYED;
                } else if (msg.content.find("Tribemember") != std::string::npos) {
                    msg.type = EventType::TRIBE_PLAYER_KILLED;
                } else { msg.type = EventType::TRIBE_DINO_KILLED; }
                break;
            }
            // white / gray events, all have a distinct identifier.
            case EventType::DINO_STARVED:
            case EventType::TRIBE_GROUP_UPDATED:
            case EventType::DINO_CRYOD: {
                if (msg.content.find("starved") != std::string::npos) {
                    msg.type = EventType::DINO_STARVED;
                } else if (msg.content.find("froze") != std::string::npos) {
                    msg.type = EventType::DINO_CRYOD;
                } else { msg.type = EventType::TRIBE_GROUP_UPDATED; }
                break;
            }
            // purple events, if a dino is killed the type of dino will be in
            // parantheses as well as the dinos tribe. Will get false matches
            // for out of tribe dinos but there is no reliable way to handle that.
            case EventType::ENEMY_DINO_KILLED:
            case EventType::CLAIMED:
            case EventType::ENEMY_PLAYER_KILLED: {
                if (msg.content.find("claimed") != std::string::npos) {
                    msg.type = EventType::CLAIMED;
                } else if (std::ranges::count(msg.content, '(') >= 2 ||
                           std::ranges::count(msg.content, ')') >= 2) {
                    msg.type = EventType::ENEMY_DINO_KILLED;
                } else { msg.type = EventType::ENEMY_PLAYER_KILLED; }
                break;
            }
            // yellow events, either unclaimed or demolished
            case EventType::UNCLAIMED:
            case EventType::DEMOLISHED:
            case EventType::PLAYER_REMOVED: {
                if (msg.content.find("removed") != std::string::npos) {
                    msg.type = EventType::PLAYER_REMOVED;
                } else if (msg.content.find("demolished") != std::string::npos) {
                    msg.type = EventType::DEMOLISHED;
                } else { msg.type = EventType::UNCLAIMED; }
                break;
            }
            default:
                // All the events that have a distinct color are already accurate.
                break;
        }
    }

    cv::Mat tribe_manager::crop_timestamp(const cv::Mat& src)
    {
        const int width = 127 + (std::to_string(last_server_info->day).length() - 1) * 10;
        return {src, cv::Rect(0, 0, width, 17)};
    }
}
