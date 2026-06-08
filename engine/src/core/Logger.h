#pragma once

#include "FileSystem.h"

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
        void Init();
        void ShutDown();
        void AddFileSink(const std::filesystem::path& logFilePath);
        void SetLevel(Level level);
        template <typename T> void Log(Level level, const T& msg);
        template <typename... TArgs> void Log(Level level, fmt::format_string<TArgs...> fmt, TArgs&&... args);
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
        static constexpr auto s_levelEnvVariableName{"LOGGER_LEVEL"};
        static constexpr auto s_name{"logger"};
        static inline const std::filesystem::path s_logFilePath{FileSystem::GetAbsolutePath("logs") /
                                                                "log.txt"};
        std::shared_ptr<spdlog::logger> m_logger{spdlog::stdout_color_mt(s_name)};
    };

    template <typename T> void Logger::Log(Level level, const T& msg)
    {
        m_logger->log(static_cast<spdlog::level::level_enum>(level), msg);
    }

    template <typename... TArgs> void Logger::Log(Level level, fmt::format_string<TArgs...> fmt,
                                                  TArgs&&... args)
    {
        m_logger->log(static_cast<spdlog::level::level_enum>(level), fmt, std::forward<TArgs>(args)...);
    }

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
