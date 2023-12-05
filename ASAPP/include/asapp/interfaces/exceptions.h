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
		std::string info;

	public:
		InterfaceError(const IInterface* _interface, std::string info);
		InterfaceError(const IInterface* _interface);

		const char* what() const noexcept override;
	};

	class InterfaceNotOpenedError : public InterfaceError
	{
	public:
		InterfaceNotOpenedError(const IInterface* _interface);
	};

	class InterfaceNotClosedError : public InterfaceError
	{
	public:
		InterfaceNotClosedError(const IInterface* _interface);
	};

	class ReceivingRemoteInventoryTimeoutError : public InterfaceError
	{
	public:
		ReceivingRemoteInventoryTimeoutError(const IInterface* _interface);
	};

}