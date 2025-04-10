#include "Script.h"

namespace Engine
{
    Script::Script(const sol::table& instance)
        : m_scriptInstance{instance}
    {
    }

    void Script::onStart() { m_scriptInstance["on_start"](m_scriptInstance); }

    void Script::onUpdate(float timeStep) { m_scriptInstance["on_update"](m_scriptInstance, timeStep); }
} // namespace Engine
