#include <Logger.h>

void Logger::init() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
}
