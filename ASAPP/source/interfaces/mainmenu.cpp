#include "asapp/interfaces/mainmenu.h"
#include "../util/util.h"
#include "asapp/game/resources.h"

using namespace asa::interfaces;



bool MainMenu::IsOpen() const
{
	return window::MatchTemplate(
		this->joinLastSession.area, resources::interfaces::join_last_session);
}

bool MainMenu::GotConnectionTimeout() const
{
	return window::MatchTemplate(
		this->accept.area, resources::interfaces::accept);
}
void MainMenu::Start() const
{
	if (!this->IsOpen()) {
		return;
	}

	do {
		window::Press("space");
	} while (!util::Await(
		[this]() { return !this->IsOpen(); }, std::chrono::seconds(5)));
}

void MainMenu::JoinLastSession()
{
	if (!this->IsOpen()) {
		return;
	}

	do {
		this->joinLastSession.Press();
	} while (!util::Await(
		[this]() { return !this->IsOpen(); }, std::chrono::seconds(5)));
}