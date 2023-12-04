#include "asapp/interfaces/serverselect.h"
#include "../core/wrappers.h"
#include "../util/util.h"
#include "asapp/game/resources.h"

using namespace asa::interfaces;


bool ServerSelect::IsBestResultSelected()
{
	static window::Color selectedColor(128, 64, 2);
	static window::Color hoveredSelectedColor(83, 39, 1);
	cv::Mat mask = window::GetMask(this->bestResult.area, selectedColor, 15);
	if (cv::countNonZero(mask) > 150) {
		return true;
	}

	mask = window::GetMask(this->bestResult.area, hoveredSelectedColor, 15);
	return cv::countNonZero(mask) > 150;
}

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

bool ServerSelect::IsJoiningServer() const
{
	static window::Color red(255, 0, 0);
	static window::Color text(193, 245, 255);
	cv::Mat mask = window::GetMask(this->joiningText2, red, 20);
	if (cv::countNonZero(mask) > 150) {
		return true;
	}

	mask = window::GetMask(this->joiningText, text, 20);
	return cv::countNonZero(mask) > 150;
}

void ServerSelect::JoinServer(const std::string& serverName)
{
	std::cout << "[+] Joining server " << serverName << "..." << std::endl;
	this->searchbar.SearchFor(serverName);
	SleepFor(std::chrono::seconds(3));

	while (!this->IsBestResultSelected()) {
		this->bestResult.Press();
		SleepFor(std::chrono::milliseconds(300));
	}


	std::cout << "\t[-] Best search result selected." << std::endl;
	while (!this->IsJoiningServer()) {
		this->joinButton.Press();
		SleepFor(std::chrono::seconds(1));
	}
	std::cout << "\t[-] Now joining session..." << std::endl;
	if (!util::Await(
			[this]() { return !this->IsOpen(); }, std::chrono::seconds(60))) {
		throw std::runtime_error("Failed to join server within 60 seconds.");
	}
	std::cout << "[+] Server joined successfully." << std::endl;
}

void ServerSelect::Refresh()
{
	this->refreshButton.Press();
	SleepFor(std::chrono::seconds(1));
}