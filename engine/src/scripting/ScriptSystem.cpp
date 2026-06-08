#include "ScriptSystem.h"

#include "ScriptInstance.h"
#include "ScriptingApi.h"
#include "core/Locator.h"
#include "resources/ResourceManager.h"

namespace Engine
{
    void ScriptSystem::Init()
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        AppendPackagePath((s_scriptingLibPath / "?.lua").string());
        m_lua.require_file("utils", s_scriptingLibPath / "utils.lua");
        m_lua.script_file(s_scriptingLibPath / "entity_script.lua");
        SetBindings();
    }

    void ScriptSystem::ShutDown() { m_scriptClasses.clear(); }

    std::optional<ScriptInstance> ScriptSystem::CreateScriptInstance(const ScriptData& scriptData,
                                                                     entt::handle entityHandle)
    {
        ScriptClass* scriptClass{GetOrLoadScriptClass(scriptData.filePath, scriptData.className)};
        if (!scriptClass) {
            return {};
        }
        const sol::optional scriptClassConstructor{scriptClass->GetConstructor()};
        if (!scriptClassConstructor) {
            Locator::GetLogger()->Warn("'{}' doesn't have a constructor", scriptClass->GetName());
            return {};
        }
        const sol::optional<sol::table> maybeScriptInstance{
            scriptClassConstructor.value()(scriptClass->GetLuaTable())};
        if (!maybeScriptInstance) {
            Locator::GetLogger()->Warn("Failed to instantiate '{}'", scriptClass->GetName());
            return {};
        }
        ScriptInstance scriptInstance{Entity{entityHandle}, scriptClass, maybeScriptInstance.value()};
        for (const auto& [name, value] : scriptData.attributes) {
            scriptInstance.SetAttribute(name, value);
        }
        return scriptInstance;
    }

    void ScriptSystem::AppendPackagePath(const std::string& packagePath)
    {
        const std::string currentPackagePath{m_lua["package"]["path"]};
        m_lua["package"]["path"] = currentPackagePath + (currentPackagePath.empty() ? "" : ";") + packagePath;
    }

    bool ScriptSystem::LuaInstanceOf(const sol::table& lhs, const sol::table& rhs)
    {
        return m_lua["utils"]["instance_of"](lhs, rhs).get<bool>();
    }

    ScriptClass* ScriptSystem::GetOrLoadScriptClass(const std::filesystem::path& filePath,
                                                    std::string_view className)
    {
        StringId scriptClassId{StringId::Intern(filePath.c_str())};
        if (const auto scriptClass{GetScriptClass(scriptClassId)}) {
            return scriptClass;
        }
        const auto absoluteFilePath{ResourceManager::GetResourcePath(filePath)};
        const auto scriptResult{m_lua.script_file(absoluteFilePath, sol::script_pass_on_error)};
        if (!scriptResult.valid()) {
            Locator::GetLogger()->Error("Error loading script {}: {} error\n\t{}", absoluteFilePath.c_str(),
                                        sol::to_string(scriptResult.status()),
                                        sol::error{scriptResult}.what());
            return {};
        }
        const sol::optional<sol::table> maybeScriptClassTable{m_lua[className]};
        if (!maybeScriptClassTable) {
            Locator::GetLogger()->Warn("Script '{}' doesn't have '{}' class", filePath.c_str(), className);
            return {};
        }
        sol::table scriptClassTable{maybeScriptClassTable.value()};
        const sol::table entityScriptClassTable{m_lua["EntityScript"]};
        if (!LuaInstanceOf(scriptClassTable, entityScriptClassTable)) {
            Locator::GetLogger()->Warn("'{}' is not an EntityScript", className);
            return {};
        }
        m_scriptClasses.emplace(scriptClassId,
                                std::make_unique<ScriptClass>(scriptClassId, className, scriptClassTable));
        return m_scriptClasses[scriptClassId].get();
    }

    sol::object ScriptSystem::GetComponent(Entity entity, const sol::table& type)
    {
        if (const auto it{m_componentTypes.find(type.pointer())}; it != m_componentTypes.end()) {
            return m_componentTypes[type.pointer()](entity);
        }
        return sol::nil;
    }

    ScriptClass* ScriptSystem::GetScriptClass(const StringId& scriptId) const
    {
        const auto scriptIterator{m_scriptClasses.find(scriptId)};
        if (scriptIterator != m_scriptClasses.end()) {
            return scriptIterator->second.get();
        }
        return nullptr;
    }

    void ScriptSystem::SetBindings()
    {
        BindCoreTypes();
        m_lua.new_usertype<ScriptingApi::Debug>("Debug", "Log", &ScriptingApi::Debug::Log);
        BindComponentTypes();
        m_lua.new_usertype<InputValue>("InputValue", "value", &InputValue::value);
    }

    void ScriptSystem::BindCoreTypes()
    {
        m_lua.new_usertype<glm::vec2>(
            "Vec2", sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(), "x",
            &glm::vec2::x, "y", &glm::vec2::y);
        m_lua.new_usertype<glm::vec3>(
            "Vec3", sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(), "x",
            &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
        m_lua.new_usertype<glm::vec4>(
            "Vec4", sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
            "x", &glm::vec4::x, "y", &glm::vec4::y, "z", &glm::vec4::z, "w", &glm::vec4::w);
        m_lua.new_usertype<StringId>("StringId", "id", &StringId::GetSid, "str",
                                     sol::resolve<std::string_view() const>(&StringId::GetString));
        m_lua.new_usertype<Entity>("Entity", "id", sol::property(&Entity::GetId));
    }

    void ScriptSystem::BindComponentTypes()
    {
        sol::usertype transform{m_lua.new_usertype<ScriptingApi::Transform>("Transform")};
        transform["position"] =
            sol::property(&ScriptingApi::Transform::GetPosition, &ScriptingApi::Transform::SetPosition);
        transform["rotation"] =
            sol::property(&ScriptingApi::Transform::GetRotation, &ScriptingApi::Transform::SetRotation);
        transform["scale"] =
            sol::property(&ScriptingApi::Transform::GetScale, &ScriptingApi::Transform::SetScale);
        transform["up"] = sol::property(&ScriptingApi::Transform::GetUp);
        transform["right"] = sol::property(&ScriptingApi::Transform::GetRight);
        transform["forward"] = sol::property(&ScriptingApi::Transform::GetForward);
        sol::usertype rigidBody2D{m_lua.new_usertype<ScriptingApi::RigidBody2D>("RigidBody2D")};
        rigidBody2D["position"] = sol::property(&ScriptingApi::RigidBody2D::GetPosition);
        rigidBody2D["rotationAngle"] = sol::property(&ScriptingApi::RigidBody2D::GetRotationAngle);
        rigidBody2D["linearVelocity"] = sol::property(&ScriptingApi::RigidBody2D::GetLinearVelocity,
                                                      &ScriptingApi::RigidBody2D::SetLinearVelocity);
        rigidBody2D["ApplyForceToCenter"] = &ScriptingApi::RigidBody2D::ApplyForceToCenter;
        rigidBody2D["ApplyTorque"] = &ScriptingApi::RigidBody2D::ApplyTorque;
        m_componentTypes.emplace(
            m_lua.create_named_table("Transform").pointer(),
            [this](const Entity entity) { return sol::make_object(m_lua, ScriptingApi::Transform{entity}); });
        m_componentTypes.emplace(m_lua.create_named_table("RigidBody2D").pointer(),
                                 [this](const Entity entity) {
                                     return sol::make_object(m_lua, ScriptingApi::RigidBody2D{entity});
                                 });
        m_lua.set_function("ApiGetComponent", [this](const Entity entity, const sol::table& table) {
            return GetComponent(entity, table);
        });
    }
} // namespace Engine
