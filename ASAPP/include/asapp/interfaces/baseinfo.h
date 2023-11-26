#pragma once
#include "asapp/game/window.h"
#include "iinterface.h"

namespace asa::interfaces
{
	class BaseInfo : public IInterface
	{
	protected:
		window::Rect area{ 747, 109, 429, 806 };

	public:
		enum Stat
		{
			HEALTH,
			WEIGHT,
		};

		virtual const int GetMaxHealth() = 0;
		virtual const int GetCurrentHealth() = 0;

		virtual const int GetMaxWeight() = 0;
		virtual const int GetCurrentWeight() = 0;

		virtual const std::string GetOwner() = 0;
		virtual const std::string GetName() = 0;
	};

}