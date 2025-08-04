#include "ScriptInstance.h"

namespace Engine
{
    ScriptInstance::ScriptInstance(const StringId& scriptClassId, const sol::table& instance)
        : m_scriptClassId{scriptClassId}, m_luaInstance{instance}
    {
    }

    void ScriptInstance::onStart() { m_luaInstance["on_start"](m_luaInstance); }

    void ScriptInstance::onUpdate(float timeStep) { m_luaInstance["on_update"](m_luaInstance, timeStep); }
} // namespace Engine
