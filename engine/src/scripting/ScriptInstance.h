#pragma once

#include "ScriptClass.h"
#include "core/Locator.h"
#include "core/StringId.h"
#include "core/Variant.h"

#include <sol/sol.hpp>
#include <string_view>
#include <utility>

namespace Engine
{
    class ScriptInstance
    {
    public:
        ScriptInstance(ScriptingApi::Entity entity, ScriptClass* scriptClass, const sol::table& luaTable);

        [[nodiscard]] const StringId& GetClassId() const { return m_scriptClass->GetId(); }
        void SetAttribute(std::string_view name, Variant value);

        template <typename... TArgs> void InvokeFunction(std::string_view functionName, TArgs&&... args);
        void InvokeOnStart();
        void InvokeOnUpdate(float timeStep);
        void InvokeOnLateUpdate(float timeStep);
        void InvokeOnCollisionEnter(Entity& otherEntity, const CollisionManifold2D& manifold,
                                    Shape2DId otherShapeId);
        void InvokeOnCollisionExit(Entity& otherEntity, Shape2DId otherShapeId);
        void InvokeOnTriggerEnter(Entity& otherEntity, Shape2DId otherShapeId);
        void InvokeOnTriggerExit(Entity& otherEntity, Shape2DId otherShapeId);

    private:
        ScriptingApi::Entity m_entity;
        sol::table m_luaTable{};
        ScriptClass* m_scriptClass{};
        bool m_enabled{true};
    };

    template <typename... TArgs> void ScriptInstance::InvokeFunction(std::string_view functionName,
                                                                     TArgs&&... args)
    {
        if (!m_enabled) {
            return;
        }
        sol::protected_function function{m_luaTable[functionName]};
        const auto result{function(m_luaTable, std::forward<TArgs>(args)...)};
        if (!result.valid()) {
            Locator::GetLogger()->Error("Error calling {}:{} on entity {}: {}", m_scriptClass->GetName(),
                                        functionName, m_entity.GetId().GetString(),
                                        sol::error{result}.what());
        }
    }
} // namespace Engine
