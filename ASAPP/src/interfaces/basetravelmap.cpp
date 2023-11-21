#include "basetravelmap.h"

bool asa::interfaces::BaseTravelMap::IsOpen()
{
	static window::Color dayText(116, 177, 186);
	auto mask = window::GetMask(dayTime, dayText, 20);

	return cv::countNonZero(mask) > 50;
}

bool asa::interfaces::BaseTravelMap::CanConfirmTarget()
{
	static window::Color readyColor(158, 88, 18);

	auto mask = window::GetMask(confirmButton.area, readyColor, 20);
	return cv::countNonZero(mask) > 50;
}

bool asa::interfaces::BaseTravelMap::HasResult()
{
	static window::Color selectedColor(255, 255, 255);
	static window::Color textColor(133, 226, 243);

	for (const components::Button& result : results) {
		auto mask = window::GetMask(result.area, selectedColor, 20);
		auto mask2 = window::GetMask(result.area, textColor, 20);

		if (cv::countNonZero(mask) > 30 || cv::countNonZero(mask2) > 30) {
			return true;
		};
	}

	return false;
}

int asa::interfaces::BaseTravelMap::CountResults()
{
	static window::Color selectedColor(255, 255, 255);
	static window::Color textColor(133, 226, 243);

	int count = 0;
	for (const components::Button& result : results) {
		auto mask = window::GetMask(result.area, selectedColor, 15);
		auto mask2 = window::GetMask(result.area, textColor, 15);

		count += (cv::countNonZero(mask) > 50 || cv::countNonZero(mask2) > 50);
	}

	return count;
}

void asa::interfaces::BaseTravelMap::SelectResult(int index)
{
	this->results[index].Press();
}
