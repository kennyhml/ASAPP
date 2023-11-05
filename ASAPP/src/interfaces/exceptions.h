#pragma once
#include "iinterface.h"
#include <exception>
#include <format>
#include <typeinfo>

namespace asa::interfaces::exceptions
{
	class InterfaceError : public std::exception
	{
		std::string message;

	public:
		InterfaceError(IInterface* _interface, std::string message);
		InterfaceError(IInterface* _interface);

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



}