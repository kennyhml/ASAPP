#include "asapp/interfaces/exceptions.h"
#include <format>
#include "../util/util.h"
#include "asapp/interfaces/iinterface.h"

namespace asa::interfaces
{
    InterfaceError::InterfaceError(const IInterface* t_interface, std::string t_info) :
        _interface(t_interface), info(std::format("InterfaceError: {}", t_info)) {};

    InterfaceError::InterfaceError(const IInterface* t_interface) : InterfaceError(
        t_interface, "Unspecified error") {};

    const char* InterfaceError::what() const noexcept { return this->info.c_str(); }

    InterfaceNotOpenedError::InterfaceNotOpenedError(const IInterface* t_interface) :
        InterfaceError(t_interface, std::format("Failed to access interface")) {};

    InterfaceNotClosedError::InterfaceNotClosedError(const IInterface* t_interface) :
        InterfaceError(t_interface, std::format("Failed to close interface")) {};

    ReceivingRemoteInventoryTimeoutError::ReceivingRemoteInventoryTimeoutError(
        const IInterface* t_interface) : InterfaceError(
        t_interface, std::format("Timed out receing remote inventory")) {};
}
