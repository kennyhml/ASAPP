#pragma once
#include "baseinfo.h"

namespace asa::interfaces
{

	class ContainerInfo : public BaseInfo
	{

	private:
		window::Rect slotsBar{ 768, 548, 382, 6 };

	public:
		virtual bool IsOpen() const { return true; }

		virtual const int GetMaxHealth() { return 0; };
		virtual const int GetCurrentHealth() { return 0; };

		virtual const int GetMaxSlots() { return 0; };
		virtual const int GetCurrentSlots() { return 0; };
		virtual const float GetFillLevel();

		virtual const int GetMaxWeight();
		virtual const int GetCurrentWeight();

		virtual const std::string GetOwner() { return "Rawr"; }
		virtual const std::string GetName() { return "Rawr"; }
	};
}