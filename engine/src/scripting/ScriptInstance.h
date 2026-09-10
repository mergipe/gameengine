#pragma once

#include "ScriptClass.h"
#include "api/Components.h"
#include "core/Variant.h"

#include <sol/sol.hpp>
#include <string_view>
#include <utility>

namespace Engine
{
    using ScriptHandle = sol::table;

    class ScriptInstance
    {
    public:
        ScriptInstance(const ScriptingApi::Entity& entity, ScriptClass* scriptClass,
                       const ScriptHandle& luaTable);

        [[nodiscard]] const ScriptClass& GetClass() const;
        [[nodiscard]] const ScriptHandle& GetHandle() const;
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
        void OnInvokeFunctionError(std::string_view functionName,
                                   const sol::protected_function_result& result);

        ScriptHandle m_handle{};
        ScriptClass* m_scriptClass{};
        ScriptingApi::Entity m_entity;
        bool m_enabled{true};
    };

    template <typename... TArgs> void ScriptInstance::InvokeFunction(std::string_view functionName,
                                                                     TArgs&&... args)
    {
        if (!m_enabled) {
            return;
        }
        sol::protected_function function{m_handle[functionName]};
        const auto result{function(m_handle, std::forward<TArgs>(args)...)};
        if (!result.valid()) {
            OnInvokeFunctionError(functionName, result);
        }
    }
} // namespace Engine
