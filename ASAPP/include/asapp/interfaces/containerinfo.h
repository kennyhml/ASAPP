#pragma once
#include "baseinfo.h"

namespace asa::interfaces
{

	class ContainerInfo : public BaseInfo
	{

	private:
		window::Rect slots_bar{ 768, 548, 382, 6 };

	public:
		virtual bool is_open() const { return true; }

		virtual int get_max_health() { return 0; };
		virtual int get_current_health() { return 0; };

		virtual int get_max_slots() { return 0; };
		virtual int get_current_slots() { return 0; };
		virtual float get_fill_level();

		virtual int get_max_weight();
		virtual int get_current_weight();

		virtual std::string get_owner() { return ""; }
		virtual std::string get_name() { return ""; }
	};
}