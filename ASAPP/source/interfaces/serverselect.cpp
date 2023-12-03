#include "asapp/interfaces/serverselect.h"
#include "../core/wrappers.h"
#include "asapp/game/resources.h"

using namespace asa::interfaces;

bool ServerSelect::IsOpen() const
{
	return window::MatchTemplate(
		this->refreshButton.area, resources::interfaces::refresh);
}

bool ServerSelect::CanJoin() const
{
	static window::Color joinableColor(153, 77, 4);
	auto masked = window::GetMask(this->joinButton.area, joinableColor, 20);

	return cv::countNonZero(masked) > 200;
}

bool ServerSelect::CanJoinLastPlayed() const
{

	static window::Color lastPlayedColor(142, 219, 231);
	auto masked = window::GetMask(
		this->joinLastPlayedButton.area, lastPlayedColor, 20);

	return cv::countNonZero(masked) > 50;
}

void ServerSelect::JoinServer(const std::string& serverName)
{
	this->searchbar.SearchFor(serverName);
}

void ServerSelect::Refresh()
{
	this->refreshButton.Press();
	SleepFor(std::chrono::seconds(1));
}