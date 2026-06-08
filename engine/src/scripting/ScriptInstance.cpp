#include "ScriptInstance.h"

namespace Engine
{
    ScriptInstance::ScriptInstance(Entity entity, ScriptClass* scriptClass, const sol::table& luaTable)
        : m_entity{entity}, m_luaTable{luaTable}, m_scriptClass{scriptClass}
    {
        m_luaTable["entity"] = m_entity;
    }

    void ScriptInstance::SetAttribute(std::string_view name, Variant value)
    {
        switch (value.type) {
        case Variant::Type::tInteger:
            m_luaTable[name] = value.asInteger;
            break;
        case Variant::Type::tFloat:
            m_luaTable[name] = value.asFloat;
            break;
        case Variant::Type::tBool:
            m_luaTable[name] = value.asBool;
            break;
        case Variant::Type::tStringId:
            m_luaTable[name] = std::string{StringId::GetString(value.asStringId)};
        default:
            break;
        }
    }

    void ScriptInstance::InvokeOnStart() { InvokeFunction("OnStart"); }

    void ScriptInstance::InvokeOnUpdate(float timeStep) { InvokeFunction("OnUpdate", timeStep); }
} // namespace Engine
