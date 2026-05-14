#include "Logger.h"

#include "StringId.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace Engine
{
    constexpr std::optional<Logger::Level> ParseLevel(const StringId& levelName)
    {
        using enum Logger::Level;
        if (levelName == SID("trace"))
            return trace;
        if (levelName == SID("debug"))
            return debug;
        if (levelName == SID("info"))
            return info;
        if (levelName == SID("warn"))
            return warn;
        if (levelName == SID("error"))
            return error;
        if (levelName == SID("critical"))
            return critical;
        return {};
    }

    constexpr std::string_view GetLevelName(Logger::Level level)
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

    void Logger::Init()
    {
        auto level{Level::trace};
        if (const char* levelNameFromEnv{std::getenv(s_levelEnvVariableName)}) {
            const auto sid{StringId(levelNameFromEnv)};
            if (const std::optional levelFromEnv{ParseLevel(sid)};
                levelFromEnv && levelFromEnv.value() > level) {
                level = levelFromEnv.value();
            }
        }
        SetLevel(level);
        Info("Logger initialized with console sink ({} level)", GetLevelName(level));
        AddFileSink(s_logFilePath);
    }

    void Logger::ShutDown() { Info("Logger shut down"); }

    void Logger::AddFileSink(const std::filesystem::path& logFilePath)
    {
        const auto file_sink{std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.c_str(), true)};
        m_logger->sinks().push_back(file_sink);
        Info("Added file sink to logger ({})", logFilePath.c_str());
    }

    void Logger::SetLevel(Level level) { m_logger->set_level(static_cast<spdlog::level::level_enum>(level)); }
} // namespace Engine
