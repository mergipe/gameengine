#ifndef SCRIPTCLASS_H
#define SCRIPTCLASS_H

#include <sol/sol.hpp>
#include <string>
#include <string_view>

namespace Engine
{
    class ScriptClass
    {
    public:
        ScriptClass(std::string_view className, const sol::table& luaClass)
            : m_className{className}, m_luaClass{luaClass}
        {
        }
        std::string_view getClassName() const { return m_className; }
        sol::optional<sol::function> getConstructor() const { return m_luaClass["new"]; }
        const sol::table& getLuaClass() { return m_luaClass; }

    private:
        std::string m_className{};
        sol::table m_luaClass{};
    };
} // namespace Engine

#endif
