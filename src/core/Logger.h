#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

namespace Engine {

class Logger {
private:
    static inline std::shared_ptr<spdlog::logger> logger{};

public:
    static void init();

    template <typename T> static void trace(const T &msg) { logger->trace(msg); }

    template <typename... Args> static void trace(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void debug(const T &msg) { logger->debug(msg); }

    template <typename... Args> static void debug(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void info(const T &msg) { logger->info(msg); }

    template <typename... Args> static void info(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void warn(const T &msg) { logger->warn(msg); }

    template <typename... Args> static void warn(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void error(const T &msg) { logger->error(msg); }

    template <typename... Args> static void error(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void critical(const T &msg) { logger->critical(msg); }

    template <typename... Args> static void critical(fmt::format_string<Args...> fmt, Args &&...args) {
        logger->critical(fmt, std::forward<Args>(args)...);
    }
};

} // namespace Engine

#endif
