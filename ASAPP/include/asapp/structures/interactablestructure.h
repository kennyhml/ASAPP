#pragma once
#include "basestructure.h"
#include "asapp/game/settings.h"
#include "asapp/interfaces/iinterface.h"

namespace asa::structures
{
    class InteractableStructure : public BaseStructure
    {
    public:
        explicit InteractableStructure(std::string name,
                                       settings::ActionMapping* t_interact_key,
                                       std::unique_ptr<interfaces::IInterface>
                                       t_interface) : BaseStructure(std::move(name)),
                                                      interface_(std::move(t_interface)),
                                                      interact_key_(t_interact_key) {}

        virtual ~InteractableStructure() = default;

        /**
         * @brief Gets the interact action mapping to interact with the structure.
         *
         * @returns A const reference to the ActionMapping to interact.
         */
        [[nodiscard]] const settings::ActionMapping& get_interact_key() const
        {
            return *interact_key_;
        }

        [[nodiscard]] virtual interfaces::IInterface* get_interface() const
        {
            return interface_.get();
        }

    protected:
        std::unique_ptr<interfaces::IInterface> interface_;

        settings::ActionMapping* interact_key_;
    };
}
