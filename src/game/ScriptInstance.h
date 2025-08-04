#ifndef SCRIPTINSTANCE_H
#define SCRIPTINSTANCE_H

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
        const StringId& getScriptClassId() const { return m_scriptClassId; }
        void onStart();
        void onUpdate(float timeStep);
        template <typename... TArgs> void call(std::string_view function, TArgs&&... args);

    private:
        StringId m_scriptClassId{};
        sol::table m_luaInstance{};
    };

    template <typename... TArgs> void ScriptInstance::call(std::string_view function, TArgs&&... args)
    {
        m_luaInstance[function](m_luaInstance, std::forward<TArgs>(args)...);
    }
} // namespace Engine

#endif
