#include "asa/core/logging.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace asa
{
    namespace
    {
        std::string registered_logger;

        std::string get_formatted_local_time()
        {
            const auto now = std::chrono::system_clock::now();
            const auto now_time = std::chrono::system_clock::to_time_t(now);

            std::tm local_time;
            localtime_s(&local_time, &now_time);
            return std::format("{:02d}-{:02d}-{:02d}", local_time.tm_mday,
                               local_time.tm_hour, local_time.tm_min);
        }

        std::shared_ptr<spdlog::sinks::sink> create_file_sink()
        {
            return std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                std::format("logs/logs - {}.log", get_formatted_local_time()), true);
        }

        std::shared_ptr<spdlog::sinks::sink> create_console_sink()
        {
            return std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }
    }

    void setup_logger(const std::string& logger_name,
                      const std::vector<spdlog::sink_ptr>& additional_sinks)
    {
        std::vector sinks{create_console_sink(), create_file_sink()};
        for (const auto& sink : additional_sinks) { sinks.push_back(sink); }

        spdlog::logger logger(logger_name, {create_console_sink(), create_file_sink()});
		logger.set_pattern("[%m-%d %H:%M:%S.%e] [%^%l%$] %v ");
        logger.set_level(spdlog::level::debug);
        logger.flush_on(spdlog::level::debug);

        registered_logger = logger_name;
        register_logger(std::make_shared<spdlog::logger>(logger));
    }

    std::shared_ptr<spdlog::logger> get_logger()
    {
        return spdlog::get(registered_logger);
    }
}
