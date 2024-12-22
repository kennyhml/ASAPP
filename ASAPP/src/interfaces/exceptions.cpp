#include "asa/interfaces/exceptions.h"
#include <format>
#include "asa/interfaces/iinterface.h"

namespace asa::interfaces
{
    InterfaceError::InterfaceError(const IInterface* t_interface, std::string t_info) :
        _interface(t_interface), info(std::format("InterfaceError: {}", t_info)) {};

    InterfaceError::InterfaceError(const IInterface* t_interface) : InterfaceError(
        t_interface, "Unspecified error") {};

    const char* InterfaceError::what() const noexcept { return this->info.c_str(); }

    InterfaceNotOpenedError::InterfaceNotOpenedError(const IInterface* t_interface) :
        InterfaceError(t_interface, std::format("Failed to open interface")) {};

    InterfaceNotClosedError::InterfaceNotClosedError(const IInterface* t_interface) :
        InterfaceError(t_interface, std::format("Failed to close interface")) {};

    ReceivingRemoteInventoryTimeoutError::ReceivingRemoteInventoryTimeoutError(
        const IInterface* t_interface) : InterfaceError(
        t_interface, std::format("Timed out receing remote inventory")) {};

    InterfaceNotOpenError::InterfaceNotOpenError(std::string t_attempted_action,
                                                 const IInterface* t_interface) :
        InterfaceError(t_interface,
                       std::format("Attempted to execute '{}' on a closed interface",
                                   std::move(t_attempted_action))) {}
}
