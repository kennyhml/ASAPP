#pragma once
#include <string>
#include <unordered_map>

namespace asa::settings
{
    struct action_mapping final
    {
        action_mapping(std::string t_name, std::string t_default);

        std::string name;
        std::string key;

        bool shift{false};
        bool ctrl{false};
        bool alt{false};
        bool cmd{false};

        [[nodiscard]] std::string to_string() const;

        static inline std::unordered_map<std::string, action_mapping*> mapped;
    };

    inline action_mapping access_inventory("AccessInventory", "F");
    inline action_mapping crouch("Crouch", "C");
    inline action_mapping prone("Prone", "X");
    inline action_mapping orbit_cam("OrbitCam", "K");
    inline action_mapping pause_menu("PauseMenu", "Escape");
    inline action_mapping ping("Ping", "MiddleMouseButton");
    inline action_mapping poop("Poop", "Semicolon");
    inline action_mapping reload("Reload", "R");
    inline action_mapping show_tribe_manager("ShowTribeManager", "L");
    inline action_mapping toggle_map("ToggleMap", "M");
    inline action_mapping transfer_item("TransferItem", "T");
    inline action_mapping use("Use", "E");
    inline action_mapping use_item1("UseItem1", "1");
    inline action_mapping use_item2("UseItem2", "2");
    inline action_mapping use_item3("UseItem3", "3");
    inline action_mapping use_item4("UseItem4", "4");
    inline action_mapping use_item5("UseItem5", "5");
    inline action_mapping use_item6("UseItem6", "6");
    inline action_mapping use_item7("UseItem7", "7");
    inline action_mapping use_item8("UseItem8", "8");
    inline action_mapping use_item9("UseItem9", "9");
    inline action_mapping use_item10("UseItem10", "0");
    inline action_mapping show_my_inventory("ShowMyInventory", "I");
    inline action_mapping drop_item("DropItem", "O");
    inline action_mapping fire("Fire", "LeftMouseButton");
    inline action_mapping targeting("Targeting", "RightMouseButton");
    inline action_mapping show_extended_info("ShowExtendedInfo", "H");
    inline action_mapping toggle_tooltip("ToggleTooltip", "G");
    inline action_mapping jump("Jump", "Space");
    inline action_mapping run("Run", "LeftShift");
    inline action_mapping console("ConsoleKeys", "Tilde");
    inline action_mapping move_to("CallMoveTo", "Comma");
    inline action_mapping aggressive("CallAggressive", "Equals");
    inline action_mapping follow_all("CallFollow", "J");
    inline action_mapping stay_all("CallStay", "U");
}
