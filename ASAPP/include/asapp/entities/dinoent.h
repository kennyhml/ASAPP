#pragma once
#include "baseentity.h"

namespace asa::entities
{
	class DinoEnt : public BaseEntity
	{
	public:
		using BaseEntity::BaseEntity;

		void exit();
	};
}