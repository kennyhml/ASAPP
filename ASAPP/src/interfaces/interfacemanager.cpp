#include "interfacemanager.h"

bool asa::interfaces::InterfaceManager::IsOpen()
{
	window::Color xColor(169, 246, 254);
	cv::Mat masked = window::GetMask(this->closeButton.area, xColor, 25);

	return cv::countNonZero(masked) > 20;
}

bool asa::interfaces::InterfaceManager::TabButton::IsSelected() const
{
	window::Color selectedColor(210, 229, 240);
	auto masked = window::GetMask(this->area, selectedColor, 25);

	return cv::countNonZero(masked) > 50;
}
