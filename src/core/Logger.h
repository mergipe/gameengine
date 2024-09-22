#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

class Logger {
public:
    template <typename T> static void info(const T &msg) { spdlog::info(msg); }

    template <typename... Args> static void info(fmt::format_string<Args...> fmt, Args &&...args) {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template <typename T> static void critical(const T &msg) { spdlog::critical(msg); }

    template <typename... Args> static void critical(fmt::format_string<Args...> fmt, Args &&...args) {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }
};

#endif
