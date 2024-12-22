#include "asa/interfaces/dinoinfo.h"


namespace asa::interfaces
{
    bool DinoInfo::is_open() const
    {
        static constexpr window::Color text_color{188, 244, 255};
        return cv::countNonZero(get_mask(them_.area, text_color, 20)) > 60;
    }

    bool DinoInfo::has_saddle_equipped() const
    {
        return is_open() && !get_slot(SADDLE).is_empty();
    }
}
