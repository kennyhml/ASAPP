#pragma once
#include <stdexcept>

namespace asa::exceptions
{
	void SetCrashAware(bool aware);
	bool GetCrashAware();

	class ShooterGameError : public std::exception
	{
	protected:
		std::string info;

	public:
		ShooterGameError(std::string info);

		const char* what();
	};

	class ServerCrashedError : public ShooterGameError
	{
	public:
		ServerCrashedError();
	};

	class GameCrashedError : public ShooterGameError
	{
	public:
		GameCrashedError();
	};
}