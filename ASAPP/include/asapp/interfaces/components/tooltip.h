#pragma once
#include "iinterfacecomponent.h"

namespace asa::interfaces::components
{
    class ItemTooltip : public IInterfaceComponent
    {
    public:
        using IInterfaceComponent::IInterfaceComponent;

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
        static ItemTooltip from_hovered(const window::Rect& area);

        /**
         * @return Gets the image of the tooltip, an image must have been taken before.
         */
        cv::Mat get_image() const { return image_; }

        /**
         * @return Takes the image of the tooltip, it must be visible at this point.
         */
        void take_image() { image_ = window::screenshot(area); }

        [[nodiscard]] std::optional<window::Rect> get_durability_area() const;


    private:
        cv::Mat image_;
    };
}
