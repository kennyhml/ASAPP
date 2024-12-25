#pragma once
#include "basestructure.h"
#include "asa/game/settings.h"
#include "asa/interfaces/asainterface.h"

namespace asa
{
    class interactable : public base_structure
    {
    public:
        interactable(std::string name, const action_mapping* t_interact_key,
                     std::unique_ptr<asainterface> t_interface)
            : base_structure(std::move(name)), interface_(std::move(t_interface)),
              interact_key_(t_interact_key) {}

        /**
         * @brief Gets the interact action mapping to interact with the structure.
         *
         * @returns A const reference to the ActionMapping to interact.
         */
        [[nodiscard]] const action_mapping& get_interact_key() const
        {
            return *interact_key_;
        }

        [[nodiscard]] virtual asainterface* get_interface() const
        {
            return interface_.get();
        }

    protected:
        std::unique_ptr<asainterface> interface_;
        const action_mapping* interact_key_;
    };
}
