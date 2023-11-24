#pragma once
#include "../interfaces/iinterface.h"


namespace asa::structures
{
	class BaseStructure
	{
	public:
		BaseStructure(
			std::string name, interfaces::IInterface* associatedInterface)
			: name(name), _interface(associatedInterface){};

		const std::string name;
		interfaces::IInterface* _interface;
	};

}