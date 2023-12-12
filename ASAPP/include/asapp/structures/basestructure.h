#pragma once
#include "../interfaces/actionwheel.h"
#include <string>

namespace asa::structures
{
	class BaseStructure
	{
	public:
		BaseStructure(std::string t_name) : name(t_name){};

		const std::string name;
		interfaces::ActionWheel action_wheel;
	};

}