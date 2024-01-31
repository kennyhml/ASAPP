#pragma once

#include <array>
#include <string>
#include "iinterface.h"
#include "components/button.h"
#include "components/searchbar.h"
#include "asapp/interfaces/components/tribelogmessage.h"

namespace asa::interfaces
{
    class TribeManager : public IInterface
    {
    public:

        using LogEntries = std::vector<components::TribeLogMessage>;
        using LogUpdateCallback = std::function<void(const LogEntries& all_events,
                                                     const LogEntries& new_events)>;

        /**
         * @brief Checks whether the tribe manager is currently open.
         */
        [[nodiscard]] bool is_open() const override;

        /**
         * @brief Opens the tribemanager.
         */
        void open();

        /**
         * @brief Closes the tribemanager (if open).
         */
        void close();

        /**
         * @brief Takes a screenshot of the current logs and processes them in a
         * seperate thread, calls a user defined callback once finished.
         *
         * @param on_finish The function to call with the update results.
         *
         * @remarks What events are new and which arent is determined by their timestamp.
         * @remarks The recent events are updated and may be used instead of a callback.
         */
        void update_tribelogs(LogUpdateCallback on_finish = nullptr);

        /**
         * @brief Takes a screenshot of the current logs.
         */
        [[nodiscard]] cv::Mat get_current_logs_image() const;

        /**
         * @brief Collects the regions of all messages in the tribe log image.
         *
         * @return A vector containing the sorted tribelog message frames top to bottom.
         */
        [[nodiscard]] std::vector<window::Rect> collect_entries(const cv::Mat& src) const;

        /**
         * @brief Gets the last 30 saved log entries.
         */
        [[nodiscard]] const LogEntries& get_logs() const { return tribelog_; }

        /**
         * @brief Checks whether a message is a new message based on its timestamp.
         *
         * @param msg The message to check whether it's new or previously known.
         */
        [[nodiscard]] bool is_new_message(const components::TribeLogMessage& msg) const;

    private:
        [[nodiscard]] components::TribeLogMessage parse(const cv::Mat& src);

        [[nodiscard]] components::TribeLogMessage::Event::EventType get_message_event(
                const cv::Mat& src) const;

        /**
         * @brief Determines the region of the timestamp in a message frame.
         *
         * @param message The rect of the message to find the timestamp in.
         *
         * @remark Useful as it allows us to process the timestamp individually.
         * @remark The once determined width is cached and reused next time.
         */
        [[nodiscard]] cv::Mat crop_timestamp(const cv::Mat& source);

        /**
         * @brief Adds a tribelog message to the history of previous messages.
         *
         * @param message The message to add.
         *
         * @remarks If the size of the tribelog history exceeds 50, the last message
         * is deleted in order to make room.
         */
        void add_message(const components::TribeLogMessage&);

        LogEntries tribelog_;
        components::Button tribe_manager_button{908, 55, 52, 52};

        int timestamp_width = 0;
        window::Rect tribe_log_area{780, 216, 380, 576};
    };

    inline std::unique_ptr<TribeManager> tribe_manager = std::make_unique<TribeManager>();
}