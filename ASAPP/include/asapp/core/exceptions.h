#pragma once
#include <stdexcept>

namespace asa::exceptions
{
	class ShooterGameError : public std::exception
	{
	public:
		ShooterGameError(std::string message);

		const char* what();
	};

	class ServerCrashedError : public ShooterGameError
	{
		using ShooterGameError::ShooterGameError;
	};

	class GameCrashedError : public ShooterGameError
	{
		using ShooterGameError::ShooterGameError;
	};
}