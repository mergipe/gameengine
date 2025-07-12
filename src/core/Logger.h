#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/sinks/stdout_color_sinks.h"
#include <filesystem>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>

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
        void addFileSink(const std::filesystem::path& logFilepath);
        void setLevel(Level level);
        template <typename T> void trace(const T& msg);
        template <typename... TArgs> void trace(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void debug(const T& msg);
        template <typename... TArgs> void debug(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void info(const T& msg);
        template <typename... TArgs> void info(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void warn(const T& msg);
        template <typename... TArgs> void warn(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void error(const T& msg);
        template <typename... TArgs> void error(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T> void critical(const T& msg);
        template <typename... TArgs> void critical(fmt::format_string<TArgs...> fmt, TArgs&&... args);

    private:
        static constexpr std::string s_loggerLevelEnvVariableName{"LOGGER_LEVEL"};
        static constexpr std::string s_loggerName{"logger"};
        std::shared_ptr<spdlog::logger> m_logger{spdlog::stdout_color_mt(s_loggerName)};
    };

    template <typename T>
    void Logger::trace(const T& msg)
    {
        m_logger->trace(msg);
    }

    template <typename... TArgs>
    void Logger::trace(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->trace(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::debug(const T& msg)
    {
        m_logger->debug(msg);
    }

    template <typename... TArgs>
    void Logger::debug(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->debug(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::info(const T& msg)
    {
        m_logger->info(msg);
    }

    template <typename... TArgs>
    void Logger::info(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->info(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::warn(const T& msg)
    {
        m_logger->warn(msg);
    }

    template <typename... TArgs>
    void Logger::warn(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->warn(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::error(const T& msg)
    {
        m_logger->error(msg);
    }

    template <typename... TArgs>
    void Logger::error(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->error(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::critical(const T& msg)
    {
        m_logger->critical(msg);
    }

    template <typename... TArgs>
    void Logger::critical(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        m_logger->critical(fmt, std::forward<TArgs>(args)...);
    }
} // namespace Engine

#endif
