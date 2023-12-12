#pragma once
#include "asapp/game/settings.h"
#include "asapp/interfaces/iinterface.h"
#include "basestructure.h"

namespace asa::structures
{
	class InteractableStructure : public BaseStructure
	{
	protected:
		const settings::ActionMapping* interact_key;

	public:
		InteractableStructure(std::string name,
			const settings::ActionMapping* t_interact_key,
			interfaces::IInterface* t_interface)
			: BaseStructure(name), _interface(t_interface),
			  interact_key(t_interact_key){};

		interfaces::IInterface* _interface;

		const settings::ActionMapping get_interact_key() const
		{
			return *(interact_key);
		}
	};
}