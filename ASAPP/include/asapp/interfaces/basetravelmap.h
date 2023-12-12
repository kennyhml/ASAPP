#pragma once
#include "components/button.h"
#include "components/searchbar.h"
#include "iinterface.h"
#include <array>
#include <string>

namespace asa::interfaces
{
	class BaseTravelMap : public IInterface
	{
	protected:
		std::array<components::Button, 12> results;
		window::Rect day_time{ 1570, 77, 272, 54 };
		components::Button confirm_button{ 1461, 931, 367, 60, 10 };

	public:
		BaseTravelMap();

		virtual bool is_open() const override;
		virtual bool can_confirm_target();
		virtual bool has_result();
		virtual bool is_result_selected(int index);

		virtual int count_results();

		virtual void select_result(int index = 0);
		virtual void go_to(const std::string& destination) = 0;
	};
}
