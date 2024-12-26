#include "asa/ui/info/dinoinfo.h"

#include "asa/utility.h"


namespace asa
{
    bool dino_info::is_open() const
    {
        static cv::Vec3b text_color{188, 244, 255};

        return utility::count_matches(them_.area, text_color, 20) > 60;
    }

    bool dino_info::has_saddle_equipped() const
    {
        return is_open() && !get_slot(SADDLE).is_empty();
    }
}
