#include "asapp/interfaces/modeselect.h"
#include "../util/util.h"
#include "asapp/game/resources.h"


namespace asa::interfaces
{
	bool ModeSelect::is_open() const
	{
		return window::match_template(
			back_button.area, resources::interfaces::back);
	}

	void ModeSelect::join_game()
	{
		if (!is_open()) {
			return;
		}
		do {
			join_game_button.press();
		} while (!util::await(
			[this]() { return !is_open(); }, std::chrono::seconds(5)));
	}
}
