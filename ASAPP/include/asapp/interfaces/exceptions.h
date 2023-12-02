#pragma once
#include "iinterface.h"
#include <exception>
#include <format>
#include <typeinfo>

namespace asa::interfaces::exceptions
{
	class InterfaceError : public std::exception
	{
		const IInterface* _interface;
		std::string message;

	public:
		InterfaceError(const IInterface* _interface, std::string message);
		InterfaceError(const IInterface* _interface);

		const char* what();
	};

	class InterfaceNotOpenedError : public InterfaceError
	{
		using InterfaceError::InterfaceError;
	};

	class InterfaceNotClosedError : public InterfaceError
	{
		using InterfaceError::InterfaceError;
	};

	class ReceivingRemoteInventoryTimeoutError : public InterfaceError
	{
		using InterfaceError::InterfaceError;
	};
}