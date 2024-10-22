#include "asapp/interfaces/basetravelmap.h"
#include <opencv2/highgui.hpp>
#include "asapp/core/state.h"
#include "asapp/util/util.h"
#include "asapp/game/resources.h"

namespace asa::interfaces
{
    DestinationNotReady::DestinationNotReady(const std::string& t_dst_name)
        : info_(std::format("No destination for '{}' is ready.", t_dst_name)) {}

    DestinationNotFound::DestinationNotFound(const std::string& t_dst_name)
        : info_(std::format("No destination for '{}' exists.", t_dst_name)) {}

    bool BaseTravelMap::DestinationButton::is_ready() const
    {
        const cv::Mat mask = window::get_mask(area, text_color, 30);
        mask |= window::get_mask(area, text_selected_color, 30);
        return cv::countNonZero(mask) > 100;
    }

    bool BaseTravelMap::DestinationButton::is_on_cooldown() const
    {
        const cv::Mat mask = window::get_mask(area, text_cooldown_color, 30);
        return cv::countNonZero(mask) > 100;
    }

    bool BaseTravelMap::DestinationButton::is_selected() const
    {
        const cv::Mat mask = window::get_mask(area, selected_color, 30);
        mask |= window::get_mask(area, hovered_selected_color, 30);

        return cv::countNonZero(mask) > 300;
    }

    void BaseTravelMap::DestinationButton::select()
    {
        do { press(); }
        while (!util::await([this]() { return is_selected(); }, std::chrono::seconds(3)));
    }

    BaseTravelMap::BaseTravelMap()
    {
        for (int i = 0; i < destination_slots_.max_size(); i++) {
            destination_slots_[i] = {89, 195 + (i * 55), 459, 55};
        }
    };

    bool BaseTravelMap::is_open() const
    {
        return window::match_template(this->day_time_, resources::interfaces::day);
    }

    bool BaseTravelMap::can_confirm_travel() const
    {
        static constexpr window::Color ready_color{158, 88, 18};

        const auto mask = window::get_mask(confirm_button.area, ready_color, 20);
        return cv::countNonZero(mask) > 50;
    }

    std::vector<BaseTravelMap::DestinationButton> BaseTravelMap::get_destinations() const
    {
        std::vector<DestinationButton> ret;

        for (const auto& roi : destination_slots_) {
            // create an imaginary button for now.
            const DestinationButton button(roi.x, roi.y);
            // button doesnt exist, end of the list reached.
            if (!(button.is_ready() || button.is_on_cooldown())) { break; }
            ret.push_back(button);
        }
        return ret;
    }

    BaseTravelMap::DestinationButton BaseTravelMap::get_ready_destination(
        const std::string& name, const bool wait_ready) const
    {
        auto results = get_destinations();
        // Give it 5 seconds for the destinations to pop up in case server is
        // lagging or something.
        const auto start = std::chrono::system_clock::now();
        while (results.empty()) {
            if (util::timedout(start, std::chrono::seconds(5))) {
                throw DestinationNotFound(name);
            }
            results = get_destinations();
        }

        const auto it = std::ranges::find_if(
            results, [](const DestinationButton& b) -> bool { return b.is_ready(); });

        if (it == results.end()) {
            if (wait_ready) {
                core::sleep_for(std::chrono::seconds(5));
                return get_ready_destination(name, wait_ready);
            }
            throw DestinationNotReady(name);
        }
        return *it;
    }
}
