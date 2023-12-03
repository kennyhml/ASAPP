#include "asapp/interfaces/serverselect.h"
#include "asapp/game/resources.h"

using namespace asa::interfaces;

bool ServerSelect::IsOpen() const
{
	return window::MatchTemplate(
		this->refreshButton.area, resources::interfaces::refresh);
}