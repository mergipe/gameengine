#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

namespace Engine
{
    class Logger
    {
    private:
        static inline std::shared_ptr<spdlog::logger> s_logger{};

    public:
        static void init();
        template <typename T>
        static void trace(const T& msg);
        template <typename... Args>
        static void trace(fmt::format_string<Args...> fmt, Args&&... args);
        template <typename T>
        static void debug(const T& msg);
        template <typename... Args>
        static void debug(fmt::format_string<Args...> fmt, Args&&... args);
        template <typename T>
        static void info(const T& msg);
        template <typename... Args>
        static void info(fmt::format_string<Args...> fmt, Args&&... args);
        template <typename T>
        static void warn(const T& msg);
        template <typename... Args>
        static void warn(fmt::format_string<Args...> fmt, Args&&... args);
        template <typename T>
        static void error(const T& msg);
        template <typename... Args>
        static void error(fmt::format_string<Args...> fmt, Args&&... args);
        template <typename T>
        static void critical(const T& msg);
        template <typename... Args>
        static void critical(fmt::format_string<Args...> fmt, Args&&... args);
    };

    template <typename T>
    void Logger::trace(const T& msg)
    {
        s_logger->trace(msg);
    }

    template <typename... Args>
    void Logger::trace(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Logger::debug(const T& msg)
    {
        s_logger->debug(msg);
    }

    template <typename... Args>
    void Logger::debug(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Logger::info(const T& msg)
    {
        s_logger->info(msg);
    }

    template <typename... Args>
    void Logger::info(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Logger::warn(const T& msg)
    {
        s_logger->warn(msg);
    }

    template <typename... Args>
    void Logger::warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Logger::error(const T& msg)
    {
        s_logger->error(msg);
    }

    template <typename... Args>
    void Logger::error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void Logger::critical(const T& msg)
    {
        s_logger->critical(msg);
    }

    template <typename... Args>
    void Logger::critical(fmt::format_string<Args...> fmt, Args&&... args)
    {
        s_logger->critical(fmt, std::forward<Args>(args)...);
    }
} // namespace Engine

#endif
