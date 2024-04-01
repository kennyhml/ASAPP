#include "asapp/interfaces/playerinfo.h"


namespace asa::interfaces
{
    bool PlayerInfo::is_open() const
    {
        static constexpr window::Color text_color{188, 244, 255};
        return cv::countNonZero(get_mask(you_.area, text_color, 20)) > 60;
    }



}
