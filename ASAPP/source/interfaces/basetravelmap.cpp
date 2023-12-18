#include "asapp/interfaces/basetravelmap.h"
#include <algorithm>
#include "../util/util.h"
#include "asapp/core/state.h"
#include "asapp/game/globals.h"
#include "asapp/game/resources.h"

namespace asa::interfaces
{
    BaseTravelMap::BaseTravelMap() : results([this]() {
        int i = 0;
        std::generate(results.begin(), results.end(), [&i]() mutable {
            return components::Button(89, 195 + (i++ * 55), 459, 55, 10);
        });
        return results;
    }()) {};

    bool BaseTravelMap::is_open() const
    {
        return window::match_template(this->day_time, resources::interfaces::day);
    }

    bool BaseTravelMap::can_confirm_target()
    {
        static window::Color ready_color(158, 88, 18);

        auto mask = window::get_mask(confirm_button.area, ready_color, 20);
        return cv::countNonZero(mask) > 50;
    }

    bool BaseTravelMap::has_result()
    {
        static window::Color selected_color(255, 255, 255);
        static window::Color text_color(133, 226, 243);

        for (const components::Button& result : results) {
            auto mask = window::get_mask(result.area, selected_color, 20);
            auto mask2 = window::get_mask(result.area, text_color, 20);

            if (cv::countNonZero(mask) > 30 || cv::countNonZero(mask2) > 30) {
                return true;
            }
        }

        return false;
    }

    bool BaseTravelMap::is_result_selected(int index)
    {
        static window::Color hovered_selected_color(83, 39, 1);
        static window::Color selected_color(128, 64, 2);

        auto roi = this->results[index].area;

        auto mask1 = window::get_mask(roi, hovered_selected_color, 30);
        auto mask2 = window::get_mask(roi, selected_color, 30);

        return cv::countNonZero(mask1) > 50 || cv::countNonZero(mask2) > 50;
    };

    int BaseTravelMap::count_results()
    {
        static window::Color selected_color(255, 255, 255);
        static window::Color text_color(133, 226, 243);

        int count = 0;
        for (const components::Button& result : results) {
            auto mask = window::get_mask(result.area, selected_color, 15);
            auto mask2 = window::get_mask(result.area, text_color, 15);

            count += (cv::countNonZero(mask) > 50 || cv::countNonZero(mask2) > 50);
        }

        return count;
    }

    void BaseTravelMap::select_result(int index)
    {
        do { this->results[index].press(); }
        while (!util::await([this, index]() { return this->is_result_selected(index); },
                            std::chrono::seconds(3)));
    }
}
