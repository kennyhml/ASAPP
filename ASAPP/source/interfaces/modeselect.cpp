#include "asapp/interfaces/modeselect.h"
#include "../util/util.h"
#include "asapp/game/resources.h"
using namespace asa::interfaces;


bool ModeSelect::IsOpen() const
{
	return window::MatchTemplate(
		this->backButton.area, resources::interfaces::back);
}


void ModeSelect::JoinGame()
{
	if (!this->IsOpen()) {
		return;
	}

	do {
		this->joinGameButton.Press();
	} while (!util::Await(
		[this]() { return !this->IsOpen(); }, std::chrono::seconds(5)));
}