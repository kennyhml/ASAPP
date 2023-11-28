#include "asapp/init.h"

#include "asapp/game/resources.h"
#include "asapp/game/settings.h"
#include "asapp/items/items.h"

bool asa::Init(std::filesystem::path configPath)
{
	return (config::Init(configPath) && resources::Init() && settings::Init() &&
			items::Init());
}
