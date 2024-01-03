#ifndef ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_
#define ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_

#include <array>
#include <string>
#include "iinterface.h"
#include "components/button.h"
#include "components/searchbar.h"

namespace asa::interfaces 
{
    class TribeManager : public IInterface 
    {
       public:
        [[nodiscard]] bool is_open() const override;
      
        void open();
        
        void close();
        
        std::vector<std::string> read_tribe_log_lines();
        
       private:
        components::Button tribe_manager_button{908, 55, 52, 52};
        
        window::Rect tribe_log_area{780, 216, 380, 576};
        window::Point2D tribe_log_line_size{380, 19.86206896551724};
        
        // todo: Maybe add a json file for these
        std::vector<std::pair<std::string, std::string>> fixes = {
            {"\n", " "},
            {"‘", "'"},
            {"’", "'"},
            {"}", ")"},
            {"{", "("},
            {"- Lvb", "- Lvl"},
            {"- Lv¥i", "- Lvl"},
            {"- Lvi", "- Lvl"},
            {" - Lv ", " - Lvl "},
            {"- Lyl", "- Lvl"},
            {"- vl ", "- Lvl "},
            {"kilied", "killed"},
            {"kilted", "killed"},
            {"kilfed", "killed"},
        };
    };

    inline std::unique_ptr<TribeManager> tribe_manager = std::make_unique<TribeManager>();
}

#endif //ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_