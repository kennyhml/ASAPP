#pragma once
#include "../items/items.h"
#include "baseentityinfo.h"

namespace asa::interfaces
{

	class PlayerInfo : public BaseEntityInfo
	{
		using BaseEntityInfo::BaseEntityInfo;


	public:
		enum SLOT
		{
			HEAD,
			TORSO,
			LEGS,
			HANDS,
			OFFHAND,
			FEET,
		};

		const bool HasEquipped(const items::Item*, SLOT slot);
	};






}