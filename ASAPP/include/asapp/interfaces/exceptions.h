#pragma once
#include "iinterface.h"
#include <exception>
#include <format>
#include <typeinfo>

namespace asa::interfaces
{
	class InterfaceError : public std::exception
	{
		const IInterface* _interface;
		std::string info;

	public:
		InterfaceError(const IInterface* t_interface, std::string t_info);
		InterfaceError(const IInterface* t_interface);

		const char* what() const noexcept override;
	};

	class InterfaceNotOpenedError : public InterfaceError
	{
	public:
		InterfaceNotOpenedError(const IInterface* t_interface);
	};

	class InterfaceNotClosedError : public InterfaceError
	{
	public:
		InterfaceNotClosedError(const IInterface* t_interface);
	};

	class ReceivingRemoteInventoryTimeoutError : public InterfaceError
	{
	public:
		ReceivingRemoteInventoryTimeoutError(const IInterface* t_interface);
	};

}