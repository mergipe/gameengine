#pragma once

#include "ScriptClass.h"
#include "api/Components.h"
#include "core/FileSystem.h"
#include "scene/Entity.h"

#include <filesystem>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <optional>

namespace Engine
{
    class ScriptInstance;

    class ScriptSystem
    {
    public:
        void Init();
        void ShutDown();

        const std::unordered_map<StringId, std::unique_ptr<ScriptClass>>& GetScriptClasses();
        std::optional<ScriptInstance> CreateScriptInstance(Entity& entity, const StringId& scriptClassId);
        std::optional<ScriptInstance> CreateScriptInstance(Entity& entity,
                                                           const ScriptClassData& scriptClassData);

    private:
        struct ComponentOperations {
            std::function<sol::table(Entity&)> get;
            std::function<sol::table(Entity&)> add;
            std::function<void(Entity&)> remove;
        };

        static inline const std::filesystem::path s_scriptingLibPath{
            FileSystem::GetAbsolutePath("scripting")};

        ScriptClass* GetScriptClass(const StringId& scriptClassId) const;
        void LoadProjectScripts();
        void AppendPackagePath(const std::string& packagePath);
        bool LuaInstanceOf(const sol::table& lhs, const sol::table& rhs);
        void LoadScriptClass(const std::filesystem::path& absoluteFilePath);
        template <typename NativeComponentType, typename ScriptComponentType> ComponentOperations
        CreateComponentOperations();
        void SetBindings();
        template <typename T> void BindVectorFunctions(sol::usertype<T> type);
        void BindCoreTypes();
        void BindComponentTypes();
        void BindPhysicsTypes();
        void SetNativeComponentsOperations();
        void SetScriptComponentOperations(const ScriptClass& scriptClass);

        [[nodiscard]] sol::table GetComponent(ScriptingApi::Entity& entity, const sol::table& componentType);
        [[nodiscard]] sol::table AddComponent(ScriptingApi::Entity&, const sol::table& componentType);
        void RemoveComponent(ScriptingApi::Entity&, const sol::table& componentType);

        std::unordered_map<StringId, std::unique_ptr<ScriptClass>> m_scriptClasses{};
        std::unordered_map<const void*, ComponentOperations> m_componentOperations{};
        sol::state m_lua{};
    };

    template <typename NativeComponentType, typename ScriptComponentType> ScriptSystem::ComponentOperations
    ScriptSystem::CreateComponentOperations()
    {
        return ComponentOperations{[this](Entity& entity) -> sol::table {
                                       if (entity.HasComponent<NativeComponentType>()) {
                                           return sol::make_object(m_lua, ScriptComponentType{&entity});
                                       }
                                       return sol::nil;
                                   },
                                   [this](Entity& entity) -> sol::table {
                                       if (entity.HasComponent<NativeComponentType>()) {
                                           return sol::nil;
                                       }
                                       entity.AddComponentOnNextStep<NativeComponentType>();
                                       return sol::make_object(m_lua, ScriptComponentType{&entity});
                                   },
                                   [this](Entity& entity) {
                                       if (entity.HasComponent<NativeComponentType>()) {
                                           entity.RemoveComponentOnNextStep<NativeComponentType>();
                                       }
                                   }};
    }

    template <typename T> void ScriptSystem::BindVectorFunctions(sol::usertype<T> type)
    {
        type["Dot"] = [](const T& v, const T& w) { return glm::dot(v, w); };
        type["Distance"] = [](const T& v, const T& w) { return glm::distance(v, w); };
        type["Length"] = [](const T& v) { return glm::length(v); };
        type["Normalize"] = [](const T& v) { return glm::normalize(v); };
        type[sol::meta_function::addition] = [](const T& v, const T& w) { return v + w; };
        type[sol::meta_function::subtraction] = [](const T& v, const T& w) { return v - w; };
        type[sol::meta_function::multiplication] = sol::overload([](const T& v, float f) { return v * f; },
                                                                 [](float f, const T& v) { return f * v; });
        type[sol::meta_function::unary_minus] = [](const T& v) { return -v; };
        type[sol::meta_function::to_string] = [](const T& v) { return glm::to_string(v); };
    }
} // namespace Engine
