#pragma once
#include "../game/window.h"
#include "iinterface.h"

namespace asa::interfaces
{

	class BaseEnityInfo : public IInterface
	{

	private:
		window::Rect area{ 747, 109, 429, 806 };
	};






}