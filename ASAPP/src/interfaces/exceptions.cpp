#include "exceptions.h"
#include "../_internal/util.h"
#include "iinterface.h"
#include <format>

using namespace asa::interfaces;

exceptions::InterfaceError::InterfaceError(
	IInterface* _interface, std::string message)
	: message(std::format("Interface Error at {}: {}",
		  _internal::_util::GetName(*_interface), message)){};

exceptions::InterfaceError::InterfaceError(IInterface* _interface)
	: message(std::format(
		  "Interface Error at {}", _internal::_util::GetName(*_interface))){};


const char* exceptions::InterfaceError::what() { return this->message.c_str(); }