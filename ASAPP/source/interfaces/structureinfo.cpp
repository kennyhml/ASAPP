#include "asapp/interfaces/containerinfo.h"

using namespace asa::interfaces;

const float ContainerInfo::GetFillLevel()
{
	auto roi = this->slotsBar;
	roi.y -= 5;

	auto masked = window::GetMask(roi, window::Color(1, 156, 136), 10);
	int greenPixels = cv::countNonZero(masked);

	return float(greenPixels) / float(this->slotsBar.width);
}

const int ContainerInfo::GetMaxWeight()
{
	return std::numeric_limits<float>::infinity();
}

const int ContainerInfo::GetCurrentWeight() { return 0; }