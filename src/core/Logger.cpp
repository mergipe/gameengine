#include "Logger.h"
#include "spdlog/common.h"
#include <cstdlib>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine
{
    void Logger::init(Level level)
    {
        s_logger = spdlog::stdout_color_mt(s_loggerName);
        const char* loggerLevelEnvValue{std::getenv(s_loggerLevelEnvVariableName.c_str())};
        if (loggerLevelEnvValue) {
            const std::optional<Level> loggerLevelFromEnv{getLevelFromString(loggerLevelEnvValue)};
            if (loggerLevelFromEnv && loggerLevelFromEnv.value() > level) {
                level = loggerLevelFromEnv.value();
            }
        }
        setLevel(level);
        Logger::trace("Logger initialized with console sink");
    }

    void Logger::init(std::filesystem::path logFilepath, Level level)
    {
        init(level);
        addFileSink(logFilepath);
    }

    void Logger::addFileSink(std::filesystem::path logFilepath)
    {
        const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.c_str(), true)};
        s_logger->sinks().push_back(file_sink);
        Logger::trace("Added file sink to logger ({})", logFilepath.c_str());
    }

    void Logger::setLevel(Level level) { s_logger->set_level(static_cast<spdlog::level::level_enum>(level)); }

    constexpr std::optional<Logger::Level> Logger::getLevelFromString(std::string_view levelStr)
    {
        if (levelStr == "trace")
            return Level::trace;
        if (levelStr == "debug")
            return Level::debug;
        if (levelStr == "info")
            return Level::info;
        if (levelStr == "warn")
            return Level::warn;
        if (levelStr == "error")
            return Level::error;
        if (levelStr == "critical")
            return Level::critical;
        return {};
    }
} // namespace Engine
