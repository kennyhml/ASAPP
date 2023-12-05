#include "asapp/interfaces/exceptions.h"
#include "../util/util.h"
#include "asapp/interfaces/iinterface.h"
#include <format>

using namespace asa::interfaces::exceptions;

InterfaceError::InterfaceError(const IInterface* _interface, std::string info)
	: _interface(_interface), info(std::format("InterfaceError: {}", info)){};

InterfaceError::InterfaceError(const IInterface* _interface)
	: InterfaceError(_interface, "Unspecified error"){};

const char* InterfaceError::what() const noexcept { return this->info.c_str(); }

InterfaceNotOpenedError::InterfaceNotOpenedError(const IInterface* _interface)
	: InterfaceError(_interface, std::format("Failed to access interface")){};

InterfaceNotClosedError::InterfaceNotClosedError(const IInterface* _interface)
	: InterfaceError(_interface, std::format("Failed to close interface")){};

ReceivingRemoteInventoryTimeoutError::ReceivingRemoteInventoryTimeoutError(
	const IInterface* _interface)
	: InterfaceError(
		  _interface, std::format("Timed out receing remote inventory")){};
