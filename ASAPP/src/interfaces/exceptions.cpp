#include "asa/interfaces/exceptions.h"
#include <format>
#include "asa/interfaces/interface.h"

namespace asa::interfaces
{
    interface_error::interface_error(const IInterface* t_interface, std::string t_info) :
        _interface(t_interface), info(std::format("InterfaceError: {}", t_info)) {};

    interface_error::interface_error(const IInterface* t_interface) : interface_error(
        t_interface, "Unspecified error") {};

    const char* interface_error::what() const noexcept { return this->info.c_str(); }

    failed_to_open::failed_to_open(const IInterface* t_interface) :
        interface_error(t_interface, std::format("Failed to open interface")) {};

    failed_to_close::failed_to_close(const IInterface* t_interface) :
        interface_error(t_interface, std::format("Failed to close interface")) {};

    receiving_remote_inventory_timeout::receiving_remote_inventory_timeout(
        const IInterface* t_interface) : interface_error(
        t_interface, std::format("Timed out receing remote inventory")) {};

    InterfaceNotOpenError::InterfaceNotOpenError(std::string t_attempted_action,
                                                 const IInterface* t_interface) :
        interface_error(t_interface,
                       std::format("Attempted to execute '{}' on a closed interface",
                                   std::move(t_attempted_action))) {}
}
