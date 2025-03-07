#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine
{
    constexpr std::optional<Logger::Level> getLevel(std::string_view levelStr)
    {
        using enum Logger::Level;
        if (levelStr == "trace")
            return trace;
        if (levelStr == "debug")
            return debug;
        if (levelStr == "info")
            return info;
        if (levelStr == "warn")
            return warn;
        if (levelStr == "error")
            return error;
        if (levelStr == "critical")
            return critical;
        return {};
    }

    constexpr std::string_view getLevelName(Logger::Level level)
    {
        using enum Logger::Level;
        switch (level) {
        case trace:
            return "trace";
        case debug:
            return "debug";
        case info:
            return "info";
        case warn:
            return "warn";
        case error:
            return "error";
        case critical:
            return "critical";
        default:
            return "";
        }
    }

    void Logger::init(Level level)
    {
        s_logger = spdlog::stdout_color_mt(s_loggerName);
        const char* loggerLevelEnvValue{std::getenv(s_loggerLevelEnvVariableName.c_str())};
        if (loggerLevelEnvValue) {
            const std::optional<Level> loggerLevelFromEnv{getLevel(loggerLevelEnvValue)};
            if (loggerLevelFromEnv && loggerLevelFromEnv.value() > level) {
                level = loggerLevelFromEnv.value();
            }
        }
        setLevel(level);
        Logger::info("Logger initialized with console sink [{}]", getLevelName(level));
    }

    void Logger::init(const std::filesystem::path& logFilepath, Level level)
    {
        init(level);
        addFileSink(logFilepath);
    }

    void Logger::addFileSink(const std::filesystem::path& logFilepath)
    {
        const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.c_str(), true)};
        s_logger->sinks().push_back(file_sink);
        Logger::info("Added file sink to logger ({})", logFilepath.c_str());
    }

    void Logger::setLevel(Level level) { s_logger->set_level(static_cast<spdlog::level::level_enum>(level)); }
} // namespace Engine
