#include "asapp/interfaces/serverselect.h"
#include "../util/util.h"
#include "asapp/core/state.h"
#include "asapp/game/resources.h"

namespace asa::interfaces
{

	bool ServerSelect::is_best_result_selected()
	{
		static window::Color selectedColor(128, 64, 2);
		static window::Color hoveredSelectedColor(83, 39, 1);
		cv::Mat mask = window::get_mask(best_result.area, selectedColor, 15);
		if (cv::countNonZero(mask) > 150) {
			return true;
		}

		mask = window::get_mask(best_result.area, hoveredSelectedColor, 15);
		return cv::countNonZero(mask) > 150;
	}

	bool ServerSelect::is_open() const
	{
		return window::match_template(
			refresh_button.area, resources::interfaces::refresh);
	}

	bool ServerSelect::can_join() const
	{
		static window::Color joinable_color(153, 77, 4);
		auto masked = window::get_mask(join_button.area, joinable_color, 20);

		return cv::countNonZero(masked) > 200;
	}

	bool ServerSelect::can_join_last_played() const
	{

		static window::Color last_played_color(142, 219, 231);
		auto masked = window::get_mask(
			join_last_played_button.area, last_played_color, 20);

		return cv::countNonZero(masked) > 50;
	}

	bool ServerSelect::is_joining_server() const
	{
		static window::Color red(255, 0, 0);
		static window::Color text(193, 245, 255);
		cv::Mat mask = window::get_mask(joining_text_2, red, 20);
		if (cv::countNonZero(mask) > 150) {
			return true;
		}

		mask = window::get_mask(joining_text, text, 20);
		return cv::countNonZero(mask) > 150;
	}

	void ServerSelect::join_server(const std::string& serverName)
	{
		std::cout << "[+] Joining server " << serverName << "..." << std::endl;
		searchbar.search_for(serverName);
		core::sleep_for(std::chrono::seconds(3));

		while (!is_best_result_selected()) {
			best_result.press();
			core::sleep_for(std::chrono::milliseconds(300));
		}


		std::cout << "\t[-] Best search result selected." << std::endl;
		while (!is_joining_server()) {
			join_button.press();
			core::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "\t[-] Now joining session..." << std::endl;
		if (!util::await(
				[this]() { return !is_open(); }, std::chrono::seconds(60))) {
			throw std::runtime_error(
				"Failed to join server within 60 seconds.");
		}
		std::cout << "[+] Server joined successfully." << std::endl;
	}

	void ServerSelect::refresh()
	{
		refresh_button.press();
		core::sleep_for(std::chrono::seconds(1));
	}





}
