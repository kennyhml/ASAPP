#include "asapp/core/init.h"
#include "asapp/game/resources.h"
#include "asapp/game/settings.h"
#include "asapp/game/window.h"
#include "asapp/items/items.h"

#include <tesseract/baseapi.h>

bool asa::Init(std::filesystem::path configPath)
{
	return (config::Init(configPath) && resources::Init() && settings::Init() &&
			items::Init() && window::Init());
}
