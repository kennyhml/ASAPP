#ifndef ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_
#define ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_

#include <array>
#include <string>
#include <utility>
#include "iinterface.h"
#include "components/button.h"
#include "components/searchbar.h"

namespace asa::interfaces
{
    class TribeLogEntryType
    {
     public:
        explicit TribeLogEntryType(std::string n, const cv::Vec3b& c, double w = 1.0) : 
            name(std::move(n)), 
            colorBGR(c),
            weight(w) {}
        
        std::string name;
        
        // The color for this line in the tribe log
        cv::Vec3b colorBGR;
        
        double weight;
    };
    
    struct TribeLogTimestamp
    {
        int32_t day;
        int32_t hour;
        int32_t minute;
        int32_t second;
  
        bool operator>(const TribeLogTimestamp& other) const {
          return to_seconds() > other.to_seconds();
        }
        
        bool operator==(const TribeLogTimestamp& other) const {
          return to_seconds() == other.to_seconds();
        }

        /// 
        /// Turn this timestamp into a comparable int that indicates the seconds (in-game)
        /// that have been elapsed since the start of the server
        ///
        [[nodiscard]] int64_t to_seconds() const {
          return second + (minute * 60) + (hour * 60 * 60) + (day * 24 * 60 * 60);
        }
    };
    
    struct TribeLogEntry
    {
        TribeLogTimestamp timestamp;
        TribeLogEntryType* type;
        std::string message;
        std::string raw;
    };

    class TribeManager : public IInterface 
    {
       public:
        TribeManager();
        
        [[nodiscard]] bool is_open() const override;
      
        void open();
        
        void close();
        
        std::vector<TribeLogEntry> read_tribe_log_lines();
        
        TribeLogEntryType* entry_killed_destroyed;
        TribeLogEntryType* entry_demolished;
        TribeLogEntryType* entry_tamed;
        TribeLogEntryType* entry_added_to_tribe;
        TribeLogEntryType* entry_starved_cryo;
        TribeLogEntryType* entry_group_updated;
        TribeLogEntryType* entry_kill;
        
       private:
        TribeLogEntry parse_entry(const std::string& raw, TribeLogEntryType* type);
      
        std::vector<TribeLogEntryType*> tribe_log_entry_types;
      
        components::Button tribe_manager_button{908, 55, 52, 52};
        
        window::Rect tribe_log_area{780, 216, 380, 576};
        window::Point2D tribe_log_line_size{380, 19.86206896551724};
        
        // todo: Maybe add a json file for these
        std::vector<std::pair<std::string, std::string>> fixes = {
            {"\n", " "},
            {"‘", "'"},
            {"’", "'"},
            {"\"", "'"},
            {"}", ")"},
            {"{", "("},
            {"- Lvb", "- Lvl"},
            {"- iLvl", "- Lvl"},
            {"- Lv¥i", "- Lvl"},
            {"- L¥l", "- Lvl"},
            {"- Lvi", "- Lvl"},
            {" - Lv ", " - Lvl "},
            {"- Lyl", "- Lvl"},
            {"- vl ", "- Lvl "},
            {"- Lyi", "- Lvl "},
            {"kilied", "killed"},
            {"kilted", "killed"},
            {"kilfed", "killed"},
        };
    };

    inline std::unique_ptr<TribeManager> tribe_manager = std::make_unique<TribeManager>();
}

#endif //ASAPP_INCLUDE_ASAPP_INTERFACES_TRIBEMANAGER_H_