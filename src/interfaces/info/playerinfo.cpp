#include "asa/interfaces/info/playerinfo.h"

#include "asa/utility.h"


namespace asa
{
    bool player_info::is_open() const
    {
        static cv::Vec3b text_color{188, 244, 255};

        return utility::count_matches(you_.area, text_color, 20) > 60;
    }
}
