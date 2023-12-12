#include "asapp/interfaces/interfacemanager.h"


namespace asa::interfaces
{
	bool InterfaceManager::is_open() const
	{
		window::Color xColor(169, 246, 254);
		cv::Mat masked = window::get_mask(close_button.area, xColor, 25);

		return cv::countNonZero(masked) > 20;
	}

	bool InterfaceManager::TabButton::is_selected() const
	{
		window::Color selectedColor(210, 229, 240);
		auto masked = window::get_mask(area, selectedColor, 25);

		return cv::countNonZero(masked) > 50;
	}
}
