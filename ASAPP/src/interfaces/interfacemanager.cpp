#include "interfacemanager.h"

bool asa::ASAInterfaceManager::IsOpen()
{
	window::Color xColor(169, 246, 254);
	cv::Mat masked = window::GetMask(this->closeButton.area, xColor, 25);

	return cv::countNonZero(masked) > 20;
}