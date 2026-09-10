#include "ScriptClass.h"

#include "ScriptInstance.h"

namespace Engine
{
    ScriptClass::ScriptClass(const StringId& id, std::string_view name, const sol::table& luaTable)
        : m_name{name}, m_id{id}, m_luaTable{luaTable}
    {
    }

    const StringId& ScriptClass::GetId() const { return m_id; }

    std::string_view ScriptClass::GetName() const { return m_name; }

    const void* ScriptClass::GetLuaTablePointer() const { return m_luaTable.pointer(); }

    std::optional<ScriptInstance> ScriptClass::ConstructInstance(Entity& entity)
    {
        sol::optional constructor{m_luaTable["new"]};
        if (!constructor) {
            return {};
        }
        const sol::optional<sol::table> scriptInstanceTable{constructor.value()(m_luaTable)};
        if (!scriptInstanceTable) {
            return {};
        }
        ScriptInstance scriptInstance{ScriptingApi::Entity{&entity}, this, scriptInstanceTable.value()};
        return scriptInstance;
    }
} // namespace Engine
