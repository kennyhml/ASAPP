#include "asapp/interfaces/tribemanager.h"
#include "asapp/game/resources.h"
#include "asapp/interfaces/exceptions.h"
#include "asapp/util/util.h"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace asa::interfaces 
{
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
    
    std::vector<std::string> TribeManager::read_tribe_log_lines() 
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
            if (window::match_template(crop, resources::interfaces::day_log, 0.5)) {
                lineMats.push_back(currentLineMats);
                currentLineMats.clear();
            }
            
            currentLineMats.push_back(crop);
            lineCount++;
        }
        
        int32_t count = 0;
        int32_t countInner = 0;
        std::vector<std::string> result;
        for (auto& lines : lineMats) {
            std::stringstream lineStr;
            for (auto& line : lines) {
                auto is_in_range = [](uint8_t out, uint8_t in) -> bool {
                    auto _in = static_cast<int32_t>(in);
                    auto _out = static_cast<int32_t>(out);
                    return _in >= _out - 100 && _in <= _out + 100;
                };
                
                auto is_color = [is_in_range](const cv::Vec3b& in, const cv::Vec3b& templ) -> bool {
                    return is_in_range(in[2], templ[2]) && is_in_range(in[1], templ[1]) && is_in_range(in[0], templ[0]);
                };
                
                //                      B     G     R
                auto red =    cv::Vec3b(0,    0,    225); // destroyed
                auto yellow = cv::Vec3b(0,    225,  225); // demolished
                auto green =  cv::Vec3b(0,    225,  0); // tamed
                auto blue =   cv::Vec3b(225,  225,  0); // added
                auto grey =   cv::Vec3b(175,  175,  175); // day/starved
                auto white =  cv::Vec3b(235,  235,  235); // group
                auto purple = cv::Vec3b(225,  0,    225); // killed
          
                auto all = { red, yellow, green, blue, grey, white, purple };
                auto src = line;
                for ( int i = 0; i < src.rows; i++ ) {
                    for ( int j = 0; j < src.cols; j++ ) {
                        for (const auto &item : all) {
                            auto in = src.at<cv::Vec3b>(i, j);
                            if (is_color(item, in)) {
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
                for (const auto &fix : fixes) {
                    std::size_t index;
                    while ((index = resultStr.find(fix.first)) != std::string::npos)
                      resultStr.replace(index, fix.first.length(), fix.second);
                }
                
                result.push_back(resultStr);
            }
            
            ++count;
            countInner = 0;
        }
        
        return result;
    }
}