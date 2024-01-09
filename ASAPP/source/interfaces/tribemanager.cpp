#include "asapp/interfaces/tribemanager.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"
#include "asapp/util/util.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <regex>

namespace asa::interfaces 
{
    TribeManager::TribeManager()
    {
        tribe_log_entry_types.push_back(entry_killed_destroyed = new TribeLogEntryType("Killed/Destroyed", cv::Vec3b(0, 0, 225)));
        tribe_log_entry_types.push_back(entry_demolished = new TribeLogEntryType("Demolished", cv::Vec3b(0, 225, 225)));
        tribe_log_entry_types.push_back(entry_tamed = new TribeLogEntryType("Tamed", cv::Vec3b(0, 225, 0)));
        tribe_log_entry_types.push_back(entry_added_to_tribe = new TribeLogEntryType("Added to tribe", cv::Vec3b(225, 225, 0)));
        tribe_log_entry_types.push_back(entry_kill = new TribeLogEntryType("Killed another player/dino", cv::Vec3b(225, 0, 225)));
        tribe_log_entry_types.push_back(entry_starved_cryo = new TribeLogEntryType("Starved/Cryod", cv::Vec3b(175, 175, 175), 0.4));
        tribe_log_entry_types.push_back(entry_group_updated = new TribeLogEntryType("Updated group", cv::Vec3b(235, 235, 235), 0.4));
    }
  
    bool TribeManager::is_open() const 
    {
        return match_template(tribe_manager_button.area, resources::interfaces::tribemanager);
    }
    
    void TribeManager::open() 
    {
        const auto start = std::chrono::system_clock::now();
        while (!is_open()) {
            window::press(settings::show_tribe_manager, true);
            if (util::await([this]() { return is_open(); }, std::chrono::seconds(5))) {
                break;
            }
        
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotOpenedError(this);
            }
        }
    }
    
    void TribeManager::close() 
    {
        const auto start = std::chrono::system_clock::now();
        while (is_open()) {
            window::press("esc", true);
            if (util::await([this]() { return !is_open(); }, std::chrono::seconds(5))) {
                return;
            }
        
            if (util::timedout(start, std::chrono::seconds(30))) {
                throw InterfaceNotClosedError(this);
            }
        }
    }
    
    std::vector<TribeLogEntry> TribeManager::read_tribe_log_lines() 
    {
        auto ss = window::screenshot(tribe_log_area);
        
        double lineCount = 0;
        std::vector<std::vector<cv::Mat>> lineMats;
        std::vector<cv::Mat> currentLineMats;
        while ((lineCount + 1) * tribe_log_line_size.y <= tribe_log_area.height) {
            cv::Rect roi(
                0.0, 
                static_cast<int32_t>(lineCount * tribe_log_line_size.y), 
                static_cast<int32_t>(tribe_log_line_size.x), 
                static_cast<int32_t>(tribe_log_line_size.y)
            );
            
            cv::Mat crop = ss(roi);
            if (window::match_template(crop, resources::interfaces::day_log, 0.6)) {
                lineMats.push_back(currentLineMats);
                currentLineMats.clear();
            }
            
            currentLineMats.push_back(crop);
            lineCount++;
        }
        
        int32_t count = 0;
        int32_t countInner = 0;
        std::vector<TribeLogEntry> result;
        for (auto& lines : lineMats) {
            std::stringstream lineStr;
            std::unordered_map<TribeLogEntryType*, double> colorCounts;
            for (auto& line : lines) {
                auto is_in_range = [](uint8_t out, uint8_t in, int32_t var) -> bool {
                    auto _in = static_cast<int32_t>(in);
                    auto _out = static_cast<int32_t>(out);
                    return _in >= _out - var && _in <= _out + var;
                };
                
                auto is_color = [is_in_range](const cv::Vec3b& in, const cv::Vec3b& templ, int32_t var) -> bool {
                    return is_in_range(in[2], templ[2], var) && is_in_range(in[1], templ[1], var) && is_in_range(in[0], templ[0], var);
                };
                
                auto src = line;
                for ( int i = 0; i < src.rows; i++ ) {
                    for ( int j = 0; j < src.cols; j++ ) {
                        for (const auto &item : tribe_log_entry_types) {
                            auto in = src.at<cv::Vec3b>(i, j);
                            if (is_color(item->colorBGR, in, 100)) {
                                if (colorCounts.contains(item)) {
                                    colorCounts[item] += 1 * item->weight;
                                } else {
                                    colorCounts[item] = 0;
                                }
                              
                                src.at<cv::Vec3b>(i, j)[2] = 255;
                                src.at<cv::Vec3b>(i, j)[1] = 255;
                                src.at<cv::Vec3b>(i, j)[0] = 255;
                            }
                        }
                    }
                }
                
                cv::Mat mask;
                cv::cvtColor(src, mask, cv::COLOR_BGR2GRAY);
                window::Color tooltip_white(255, 255, 255);
                mask = window::get_mask(mask, tooltip_white, 50);
                
                window::set_tesseract_image(mask);
                window::tessEngine->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
                
                std::string resultStr = window::tessEngine->GetUTF8Text();
                lineStr << resultStr;
                ++countInner;
            }
            
            if (count > 0) {
                std::string resultStr = lineStr.str();
  
                // Find which entry this relates to by checking which color is most prominent in this line
                TribeLogEntryType* type = entry_group_updated;
                double typeCount = 0;
                for (const auto &item : colorCounts) {
                    if (item.second > typeCount) {
                        type = item.first;
                        typeCount = item.second;
                    }
                }
              
                auto entry = parse_entry(resultStr, type);
                std::cout << count << ": " << entry.raw << std::endl;
                result.push_back(entry);
            }
            
            ++count;
            countInner = 0;
        }
        
        return result;
    }
    
    TribeLogEntry TribeManager::parse_entry(const std::string& raw, TribeLogEntryType* type) 
    {
        std::string in = raw;
        for (const auto &fix : fixes) {
            std::size_t index;
            while ((index = in.find(fix.first)) != std::string::npos) {
                in.replace(index, fix.first.length(), fix.second);
            }
        }  
        
        TribeLogTimestamp timestamp{};
        static const std::regex pattern(R"(Day ([0-9]+), ([0-9]+):([0-9]+):([0-9]+): (.*))");

        std::string line;
        std::smatch sm;
        if (regex_search(in, sm, pattern)) {
          if (sm.size() > 4) {
            timestamp.day = stoi(sm[1]);
            timestamp.hour = stoi(sm[2]);
            timestamp.minute = stoi(sm[3]);
            timestamp.second = stoi(sm[4]);
            line = sm[5];
          }
        }
        
        return { timestamp, type, line, in };
    }
}