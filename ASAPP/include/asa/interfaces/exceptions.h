#pragma once
#include <format>
#include "iinterface.h"
#include "asa/core/exceptions.h"

namespace asa::interfaces
{
    class InterfaceError : public core::ASAPPError
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

    /**
     * @brief Thrown when an action is attempted on an interface that requires
     * the it to be open, but it was not.
     */
    class InterfaceNotOpenError : public InterfaceError
    {
    public:
        InterfaceNotOpenError(std::string t_attempted_action,
                              const IInterface* t_interface);
    };
}
