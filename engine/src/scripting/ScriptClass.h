#pragma once

#include "core/StringId.h"

#include <sol/sol.hpp>
#include <string>
#include <string_view>

namespace Engine
{
    class ScriptClass
    {
    public:
        ScriptClass(const StringId& id, std::string_view name, const sol::table& luaTable)
            : m_name{name}, m_id{id}, m_luaTable{luaTable}
        {
        }
        [[nodiscard]] const StringId& GetId() const { return m_id; }
        [[nodiscard]] std::string_view GetName() const { return m_name; }
        [[nodiscard]] sol::optional<sol::function> GetConstructor() const { return m_luaTable["new"]; }
        const sol::table& GetLuaTable() { return m_luaTable; }

    private:
        std::string m_name{};
        StringId m_id{};
        sol::table m_luaTable{};
    };
} // namespace Engine
