#include "asapp/interfaces/mainmenu.h"
#include "asapp/game/resources.h"

const bool asa::interfaces::MainMenu::IsOpen() const
{
	return window::MatchTemplate(this->esc.area, resources::interfaces::esc);
}