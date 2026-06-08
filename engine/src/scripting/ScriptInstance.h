#ifndef SCRIPT_INSTANCE_H
#define SCRIPT_INSTANCE_H

#include "core/Locator.h"
#include "core/StringId.h"
#include "scene/Entity.h"

#include <sol/sol.hpp>
#include <string_view>
#include <utility>

namespace Engine
{
    class ScriptInstance
    {
    public:
        ScriptInstance(Entity entity, ScriptClass* scriptClass, const sol::table& luaTable);
        [[nodiscard]] const StringId& GetClassId() const { return m_scriptClass->GetId(); }
        void SetAttribute(std::string_view name, Variant value);
        void InvokeOnStart();
        void InvokeOnUpdate(float timeStep);
        template <typename... TArgs> void InvokeFunction(std::string_view functionName, TArgs&&... args);

    private:
        Entity m_entity;
        sol::table m_luaTable{};
        ScriptClass* m_scriptClass{};
    };

    template <typename... TArgs> void ScriptInstance::InvokeFunction(std::string_view functionName,
                                                                     TArgs&&... args)
    {
        sol::protected_function function{m_luaTable[functionName]};
        const auto result{function(m_luaTable, std::forward<TArgs>(args)...)};
        if (!result.valid()) {
            Locator::GetLogger()->Error("Error calling {}:{} on entity {}: {}", m_scriptClass->GetName(),
                                        functionName, m_entity.GetId().GetString(),
                                        sol::error{result}.what());
        }
    }
} // namespace Engine

#endif
