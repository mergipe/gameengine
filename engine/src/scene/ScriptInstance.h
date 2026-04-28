#ifndef SCRIPT_INSTANCE_H
#define SCRIPT_INSTANCE_H

#include "core/StringId.h"

#include <sol/sol.hpp>
#include <string_view>
#include <utility>

namespace Engine
{
    class ScriptInstance
    {
    public:
        ScriptInstance(const StringId& scriptClassId, const sol::table& instance);
        const StringId& GetScriptClassId() const { return m_scriptClassId; }
        void OnStart();
        void OnUpdate(float timeStep);
        template <typename... TArgs> void Call(std::string_view function, TArgs&&... args);

    private:
        StringId m_scriptClassId{};
        sol::table m_luaInstance{};
    };

    template <typename... TArgs> void ScriptInstance::Call(std::string_view function, TArgs&&... args)
    {
        m_luaInstance[function](m_luaInstance, std::forward<TArgs>(args)...);
    }
} // namespace Engine

#endif
