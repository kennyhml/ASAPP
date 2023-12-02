#pragma once
#include "asapp/game/settings.h"
#include "asapp/interfaces/iinterface.h"
#include "basestructure.h"

namespace asa::structures
{
	class InteractableStructure : public BaseStructure
	{
	protected:
		const settings::ActionMapping* interactKey;

	public:
		InteractableStructure(std::string name,
			const settings::ActionMapping* interactKey,
			interfaces::IInterface* _interface)
			: BaseStructure(name), _interface(_interface),
			  interactKey(interactKey){};

		interfaces::IInterface* _interface;

		const settings::ActionMapping GetInteractKey() const
		{
			return *(this->interactKey);
		}
	};
}