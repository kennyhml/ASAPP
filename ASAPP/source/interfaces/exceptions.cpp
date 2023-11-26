#include "asapp/interfaces/exceptions.h"
#include "../util/util.h"
#include "asapp/interfaces/iinterface.h"
#include <format>

using namespace asa::interfaces;

exceptions::InterfaceError::InterfaceError(
	IInterface* _interface, std::string message)
	: message(std::format(
		  "Interface Error at {}: {}", util::GetName(*_interface), message)){};

exceptions::InterfaceError::InterfaceError(IInterface* _interface)
	: message(
		  std::format("Interface Error at {}", util::GetName(*_interface))){};


const char* exceptions::InterfaceError::what() { return this->message.c_str(); }