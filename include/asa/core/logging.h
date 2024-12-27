#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

namespace asa
{
    /**
     * @brief Sets up the logger to use for any actions taken within the library.
     *
     * @param logger_name The name to register the logger as.
     * @param additional_sinks Additional sinks to register on the logger for customization.
     */
    void setup_logger(
        const std::string& logger_name = "ASAPP-Logger",
        const std::vector<spdlog::sink_ptr >& additional_sinks = {}
    );

    /**
     * @brief Gets the logger used by the asa library.
     */
    std::shared_ptr<spdlog::logger> get_logger();
}
