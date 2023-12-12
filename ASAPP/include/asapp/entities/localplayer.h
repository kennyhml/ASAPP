#pragma once
#include "asapp/interfaces/localinventory.h"
#include "asapp/interfaces/spawnmap.h"
#include "asapp/structures/basestructure.h"
#include "asapp/structures/container.h"
#include "asapp/structures/simplebed.h"
#include "asapp/structures/teleporter.h"
#include "playerent.h"

namespace asa::entities
{
	class LocalPlayer : public PlayerEnt
	{
	public:
		LocalPlayer(std::unique_ptr<interfaces::LocalInventory> t_inventory)
			: PlayerEnt("You", std::move(t_inventory)){};

		interfaces::LocalInventory* get_inventory() const override;

		[[nodiscard]] bool is_alive() const;
		[[nodiscard]] bool is_out_of_water() const;
		[[nodiscard]] bool is_out_of_food() const;
		[[nodiscard]] bool is_overweight() const;
		[[nodiscard]] bool received_item(items::Item* = nullptr) const;
		[[nodiscard]] bool deposited_item(items::Item* = nullptr) const;
		[[nodiscard]] bool is_in_spawn_animation() const;
		[[nodiscard]] bool is_in_travel_screen() const;
		[[nodiscard]] bool can_access_bed() const;
		[[nodiscard]] bool can_access_inventory() const;
		[[nodiscard]] bool can_use_default_teleport() const;

		bool deposit_into_dedi(items::Item*, int* depositedAmountOut);
		bool withdraw_from_dedi(items::Item*, int* withdrawAmountOut);
		bool get_amount_added(items::Item&, int& amountOut);
		bool get_amount_removed(items::Item&, int& amountOut);

		bool can_access(const structures::BaseStructure&) const;
		bool can_access(const entities::BaseEntity&) const;

		void access(const entities::BaseEntity&) const;
		void access(const structures::Container&) const;
		void access(const structures::InteractableStructure&) const;

		void fast_travel_to(const structures::SimpleBed&);
		void teleport_to(const structures::Teleporter&, bool isDefault = false);
		void lay_on(const structures::SimpleBed&);
		void get_off_bed();
		void suicide();

		void jump() { window::Press(settings::jump); }
		void crouch() { window::Press(settings::crouch); }
		void prone() { window::Press(settings::prone); }
		void stand_up() { window::Press(settings::run); }

		void turn_right(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void turn_left(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void turn_up(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));
		void turn_down(int degree = 90,
			std::chrono::milliseconds delay = std::chrono::milliseconds(100));

		void walk_forward(std::chrono::milliseconds duration);
		void walk_left(std::chrono::milliseconds duration);
		void walk_right(std::chrono::milliseconds duration);
		void walk_back(std::chrono::milliseconds duration);

		void equip(items::Item* item, interfaces::PlayerInfo::Slot targetSlot);
		void unequip(interfaces::PlayerInfo::Slot targetSlot);

		void look_fully_down();
		void look_fully_up();

	private:
		void pass_travel_screen(bool in = true, bool out = true);
		void pass_teleport_screen(bool allowAccessFlag = false);
	};

	inline std::unique_ptr<LocalPlayer> local_player =
		std::make_unique<LocalPlayer>(
			std::make_unique<interfaces::LocalInventory>());

}