#include "asa/interfaces/components/tribelog_message.h"
#include <regex>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace asa
{
    namespace
    {
        std::string fill_zeros(const int number, const int zeros)
        {
            std::ostringstream oss;
            oss << std::setw(zeros) << std::setfill('0') << number;
            return oss.str();
        }

        std::regex TIMESTAMP_PATTERN(
            R"(Day ([0-9]+), ([0-9]+):([0-9]+):([0-9]+)(: (.*))?)");
    }

    int64_t tribelog_message::timestamp::sum() const
    {
        return second + (minute * 60) + (hour * 3600) + (day * 86400);
    }


    std::string tribelog_message::timestamp::to_string() const
    {
        return std::format("Day {}, {}:{}:{}", day, fill_zeros(hour, 2),
                           fill_zeros(minute, 2), fill_zeros(second, 2));
    }


    tribelog_message::timestamp tribelog_message::timestamp::parse(const std::string& raw)
    {
        std::smatch sm;
        if (regex_search(raw, sm, TIMESTAMP_PATTERN) && sm.size() > 4) {
            return {
                stoi(sm[1]), std::min(23, std::stoi(sm[2])),
                std::min(59, std::stoi(sm[3])), std::min(59, std::stoi(sm[4]))
            };
        }
        std::cout << "[!] Failed to parse " << raw << std::endl;
        return {0, 0, 0, 0};
    }
}
