#pragma once
#include "interface_component.h"

namespace asa
{
    class item_tooltip : public interface_component
    {
    public:
        using interface_component::interface_component;

        /**
         * @brief Constructs a new item tooltip from an area while hovered meaning the
         * image for the tooltip is automatically snapped during construction.
         *
         * @warning This constructor should only be used when the resulting tooltip
         * is currently hovered.
         *
         * @param area The area of the tooltip to be created (thats hovered)
         * @return
         */
        static item_tooltip from_hovered(const cv::Rect& area);

        /**
         * @return Gets the image of the tooltip, an image must have been taken before.
         */
        cv::Mat get_image() const { return image_; }

        /**
         * @return Takes the image of the tooltip, it must be visible at this point.
         */
        void take_image() { image_ = screenshot(area); }

        [[nodiscard]] std::optional<cv::Rect> get_durability_area() const;


    private:
        cv::Mat image_;
    };
}
