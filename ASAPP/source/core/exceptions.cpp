#include "asapp/core/exceptions.h"


asa::exceptions::ShooterGameError::ShooterGameError(std::string info)
	: info(info){};

const char* asa::exceptions::ShooterGameError::what()
{
	return ("ShooterGameError: " + this->info).c_str();
}

asa::exceptions::ServerCrashedError::ServerCrashedError()
	: ShooterGameError("Server or connection crashed."){};

asa::exceptions::GameCrashedError::GameCrashedError()
	: ShooterGameError("The game has crashed"){};
