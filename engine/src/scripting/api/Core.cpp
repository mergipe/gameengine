#include "Core.h"

#include "core/Locator.h"

namespace Engine::ScriptingApi
{
    void Debug::Log(std::string_view message) { Locator::GetLogger()->Debug(message); }

    Engine::Timer::Ticks Timer::GetMilliseconds() { return Engine::Timer::GetMilliseconds(); }
} // namespace Engine::ScriptingApi
