#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine
{
    class Logger
    {
    public:
        enum class Level {
            trace = spdlog::level::level_enum::trace,
            debug = spdlog::level::level_enum::debug,
            info = spdlog::level::level_enum::info,
            warn = spdlog::level::level_enum::warn,
            error = spdlog::level::level_enum::err,
            critical = spdlog::level::level_enum::critical,
        };
        explicit Logger(Level level = Level::trace);
        explicit Logger(const std::filesystem::path& logFilepath, Level level = Level::trace);
        void AddFileSink(const std::filesystem::path& logFilepath);
        void SetLevel(Level level);
        template <typename T> void Trace(const T& msg);
        template <typename... TArgs> void Trace(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void Debug(const T& msg);
        template <typename... TArgs> void Debug(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void Info(const T& msg);
        template <typename... TArgs> void Info(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void Warn(const T& msg);
        template <typename... TArgs> void Warn(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void Error(const T& msg);
        template <typename... TArgs> void Error(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void Critical(const T& msg);
        template <typename... TArgs> void Critical(fmt::format_string<TArgs...> fmt, TArgs&&... args);

    private:
        static constexpr std::string s_loggerLevelEnvVariableName{"LOGGER_LEVEL"};
        static constexpr std::string s_loggerName{"logger"};
        std::shared_ptr<spdlog::logger> m_logger{spdlog::stdout_color_mt(s_loggerName)};
    };

    template <typename T>
    void Logger::Trace(const T& msg)
    {
        m_logger->trace(msg);
    }

    template <typename... TArgs>
    void Logger::Trace(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->trace(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::Debug(const T& msg)
    {
        m_logger->debug(msg);
    }

    template <typename... TArgs>
    void Logger::Debug(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->debug(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::Info(const T& msg)
    {
        m_logger->info(msg);
    }

    template <typename... TArgs>
    void Logger::Info(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->info(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::Warn(const T& msg)
    {
        m_logger->warn(msg);
    }

    template <typename... TArgs>
    void Logger::Warn(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->warn(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::Error(const T& msg)
    {
        m_logger->error(msg);
    }

    template <typename... TArgs>
    void Logger::Error(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->error(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::Critical(const T& msg)
    {
        m_logger->critical(msg);
    }

    template <typename... TArgs>
    void Logger::Critical(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->critical(fmt, std::forward<TArgs>(args)...);
    }
} // namespace Engine

#endif
