#include "asapp/core/exceptions.h"


asa::exceptions::ShooterGameError::ShooterGameError(std::string info)
	: info("ShooterGameError: " + info){};

const char* asa::exceptions::ShooterGameError::what()
{
	return this->info.c_str();
}

asa::exceptions::ServerCrashedError::ServerCrashedError()
	: ShooterGameError("Server or internet connection has crashed."){};

asa::exceptions::GameCrashedError::GameCrashedError()
	: ShooterGameError("The game has run into a fatal error."){};
