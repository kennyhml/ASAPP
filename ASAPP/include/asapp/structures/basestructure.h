#pragma once
#include "../interfaces/actionwheel.h"
#include <string>

namespace asa::structures
{
	class BaseStructure
	{
	public:
		BaseStructure(std::string name) : name(name){};

		const std::string name;
		interfaces::ActionWheel actionWheel;
	};

}