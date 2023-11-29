#include "asapp/interfaces/actionwheel.h"
#include "asapp/game/resources.h"

bool asa::interfaces::ActionWheel::IsOpen()
{
	window::Color white(126, 216, 236);

	auto masked = window::GetMask(this->nameArea, white, 30);
	return cv::countNonZero(masked) > 100;
}


void asa::interfaces::ActionWheel::SelectLayOn()
{
	auto matchLoc = window::LocateTemplate(this->area, resources::lay_on, 0.9);

	if (!matchLoc.has_value()) {
		std::cerr << "[!] Lay on option not found! " << std::endl;
		return;
	}

	auto randPoint = matchLoc.value().GetRandLocation(5);
	window::SetMousePos(this->area.x + randPoint.x, this->area.y + randPoint.y);
	std::this_thread::sleep_for(std::chrono::milliseconds(400));
	controls::MousePress(controls::LEFT);
}
