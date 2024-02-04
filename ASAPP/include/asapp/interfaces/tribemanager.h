#pragma once
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
        using LogUpdateCallback = std::function<void(const LogEntries& all,
                                                     const LogEntries& new_)>;

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
         * seperate thread, calls the given callback with the results once finished.
         *
         * @param on_finish The function to call with the update results.
         * @param receive_for The duration to let the game receive new events in seconds.
         *
         * @remark What events are new and which arent is determined by their timestamp.
         * @remark The stored logs updated and may be used instead of a callback.
         */
        void update_tribelogs(const LogUpdateCallback& on_finish,
                              std::chrono::seconds receive_for = std::chrono::seconds(1));

        /**
         * @brief Takes a screenshot of the current logs.
         */
        [[nodiscard]] cv::Mat get_current_logs_image() const;

        /**
         * @brief Gets the last 50 saved log entries.
         */
        [[nodiscard]] const LogEntries& get_logs() const { return tribelog_; }

    private:
        /**
         * @brief Parses a TribeLogMessage from an image of a message.
         *
         * @param src The image of the message to parse.
         *
         * @return A TribeLogMessage representing the messages data.
         *
         * @remark If the message could not be determined, it will hold invalid data.
         */
        [[nodiscard]] components::TribeLogMessage parse(const cv::Mat& src);

        /**
         * @brief Collects the regions of all messages in the tribe log image.
         *
         * @param src The tribelog image to collect all the entries in.
         *
         * @return A vector containing the sorted tribelog message from bottom to top.
         */
        [[nodiscard]] std::vector<window::Rect> collect_entries(const cv::Mat& src) const;

        /**
         * @brief Roughly determines a messages event type based on the dominant color.
         *
         * @param src An image of the message to get the event type of.
         *
         * @return The event type of the message or UNKNOWN.
         *
         * @remark One dominant color can mean multiple events (such as red being used for
         * destroyed, player killed, dino killed). To get the final specific event type,
         * the contents of the message must be evaluated instead.
         */
        [[nodiscard]] components::TribeLogMessage::EventType get_message_event(
            const cv::Mat& src) const;

        /**
         * @brief Evaluates the specific event of the message based on it's contents.
         * If the content could not be evaluated further, the type is set to unknown.
         *
         * @remark The content of the given message must be determined beforehand.
         * @remark See get_message_event for an explanation why this is needed.
         *
         */
        void evaluate_message_event(components::TribeLogMessage&) const;

        /**
         * @brief Determines the region of the timestamp in a message frame.
         *
         * @param src The image of the message to find the timestamp in.
         *
         * @remark Useful as it allows us to process the timestamp individually.
         * @remark The once determined width is cached and reused next time.
         */
        [[nodiscard]] cv::Mat crop_timestamp(const cv::Mat& src);

        /**
         * @brief Adds a tribelog message to the history of previous messages.
         *
         * @param message The message to add.
         *
         * @remarks If the size of the tribelog history exceeds 50, the last message
         * is deleted in order to make room.
         */
        void add_message(const components::TribeLogMessage& message);

        /**
         * @brief Checks whether a message is a new message based on its timestamp.
         *
         * @param msg The message to check whether it's new or previously known.
         * @param allow_equal Allow same time days to be considered new.
         */
        [[nodiscard]] bool is_new_message(components::TribeLogMessage::Timestamp msg,
                                          bool allow_equal) const;

        LogEntries tribelog_;
        components::Button close_button_{1781, 49, 36, 33};
        components::Button tribe_manager_button{908, 55, 52, 52};

        window::Rect tribe_log_area{780, 216, 380, 576};
    };


    inline std::unique_ptr<TribeManager> tribe_manager = std::make_unique<TribeManager>();
}
