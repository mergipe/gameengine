#pragma once

#include "core/Timer.h"

#include <string_view>

namespace Engine::ScriptingApi
{
    class Debug
    {
    public:
        static void Log(std::string_view message);
    };

    class Timer
    {
    public:
        static Engine::Timer::Ticks GetMilliseconds();
    };
} // namespace Engine::ScriptingApi
