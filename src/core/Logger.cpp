#include <Logger.h>
#include <memory>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void Logger::init() {
    const auto console_sink{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};
    console_sink->set_level(spdlog::level::trace);
    const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true)};
    file_sink->set_level(spdlog::level::trace);
    const std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
    spdlog::register_logger(logger);
    spdlog::cfg::load_env_levels();
    Logger::trace("Logger initialized");
}
