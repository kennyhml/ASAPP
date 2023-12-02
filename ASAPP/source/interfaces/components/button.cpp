#include "asapp/interfaces/components/button.h"
#include "asapp/game/controls.h"

using namespace asa::interfaces;

void components::Button::Press()
{

	auto loc = this->area.GetRandLocation(this->borderPadding);

	window::PostMousePressAt(loc, controls::MouseButton::LEFT);

	this->lastPressed = std::chrono::system_clock::now();
	this->lastButtonPress = lastPressed;
};