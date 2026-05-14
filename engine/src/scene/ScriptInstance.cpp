#include "ScriptInstance.h"

namespace Engine
{
    ScriptInstance::ScriptInstance(const StringId& scriptClassId, const sol::table& instance)
        : m_scriptClassId{scriptClassId}, m_luaInstance{instance}
    {
    }

    void ScriptInstance::OnStart() { m_luaInstance["OnStart"](m_luaInstance); }

    void ScriptInstance::OnUpdate(float timeStep) { m_luaInstance["OnUpdate"](m_luaInstance, timeStep); }
} // namespace Engine
