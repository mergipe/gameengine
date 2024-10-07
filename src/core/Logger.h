#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

namespace Engine
{
    class Logger
    {
    private:
        static inline std::shared_ptr<spdlog::logger> s_logger{std::make_shared<spdlog::logger>("empty")};

    public:
        static void init();
        template <typename T>
        static void trace(const T& msg);
        template <typename... TArgs>
        static void trace(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T>
        static void debug(const T& msg);
        template <typename... TArgs>
        static void debug(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T>
        static void info(const T& msg);
        template <typename... TArgs>
        static void info(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T>
        static void warn(const T& msg);
        template <typename... TArgs>
        static void warn(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T>
        static void error(const T& msg);
        template <typename... TArgs>
        static void error(fmt::format_string<TArgs...> fmt, TArgs&&... args);
        template <typename T>
        static void critical(const T& msg);
        template <typename... TArgs>
        static void critical(fmt::format_string<TArgs...> fmt, TArgs&&... args);
    };

    template <typename T>
    void Logger::trace(const T& msg)
    {
        s_logger->trace(msg);
    }

    template <typename... TArgs>
    void Logger::trace(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->trace(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::debug(const T& msg)
    {
        s_logger->debug(msg);
    }

    template <typename... TArgs>
    void Logger::debug(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->debug(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::info(const T& msg)
    {
        s_logger->info(msg);
    }

    template <typename... TArgs>
    void Logger::info(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->info(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::warn(const T& msg)
    {
        s_logger->warn(msg);
    }

    template <typename... TArgs>
    void Logger::warn(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->warn(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::error(const T& msg)
    {
        s_logger->error(msg);
    }

    template <typename... TArgs>
    void Logger::error(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->error(fmt, std::forward<TArgs>(args)...);
    }

    template <typename T>
    void Logger::critical(const T& msg)
    {
        s_logger->critical(msg);
    }

    template <typename... TArgs>
    void Logger::critical(fmt::format_string<TArgs...> fmt, TArgs&&... args)
    {
        s_logger->critical(fmt, std::forward<TArgs>(args)...);
    }
} // namespace Engine

#endif
