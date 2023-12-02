#include "asapp/interfaces/exceptions.h"
#include "../util/util.h"
#include "asapp/interfaces/iinterface.h"
#include <format>


asa::interfaces::exceptions::InterfaceError::InterfaceError(
	const IInterface* _interface, std::string message)
	: message(std::format(
		  "Interface Error at {}: {}", util::GetName(*_interface), message)){};

asa::interfaces::exceptions::InterfaceError::InterfaceError(
	const IInterface* _interface)
	: message(
		  std::format("Interface Error at {}", util::GetName(*_interface))){};


const char* asa::interfaces::exceptions::InterfaceError::what()
{
	return this->message.c_str();
}