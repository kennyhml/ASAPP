#include "asapp/core/exceptions.h"

inline bool crashAware = false;

void asa::exceptions::SetCrashAware(bool aware) { crashAware = aware; }
bool asa::exceptions::GetCrashAware() { return crashAware; }

asa::exceptions::ShooterGameError::ShooterGameError(std::string info)
	: info("ShooterGameError: " + info){};

const char* asa::exceptions::ShooterGameError::what() const noexcept
{
	return this->info.c_str();
}

asa::exceptions::ServerCrashedError::ServerCrashedError()
	: ShooterGameError("Server / connection crashed."){};

asa::exceptions::GameCrashedError::GameCrashedError()
	: ShooterGameError("The game has crashed."){};
