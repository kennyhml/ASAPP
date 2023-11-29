#include "asapp/interfaces/actionwheel.h"

bool asa::interfaces::ActionWheel::IsOpen()
{
	window::Color white(126, 216, 236);

	auto masked = window::GetMask(this->nameArea, white, 30);
	return cv::countNonZero(masked) > 100;
}
