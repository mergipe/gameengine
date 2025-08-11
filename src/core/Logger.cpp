#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace Engine
{
    constexpr std::optional<Logger::Level> parseLevel(std::string_view levelStr)
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

    Logger::Logger(Level level)
    {
        if (const char* loggerLevelEnvValue{std::getenv(s_loggerLevelEnvVariableName.c_str())}) {
            const std::optional<Level> loggerLevelFromEnv{parseLevel(loggerLevelEnvValue)};
            if (loggerLevelFromEnv && loggerLevelFromEnv.value() > level) {
                level = loggerLevelFromEnv.value();
            }
        }
        setLevel(level);
        info("Logger initialized with console sink ({} level)", getLevelName(level));
    }

    Logger::Logger(const std::filesystem::path& logFilepath, Level level)
        : Logger{level}
    {
        addFileSink(logFilepath);
    }

    void Logger::addFileSink(const std::filesystem::path& logFilepath)
    {
        const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.c_str(), true)};
        m_logger->sinks().push_back(file_sink);
        info("Added file sink to logger ({})", logFilepath.c_str());
    }

    void Logger::setLevel(Level level) { m_logger->set_level(static_cast<spdlog::level::level_enum>(level)); }
} // namespace Engine
