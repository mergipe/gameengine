#include "ScriptSystem.h"

#include "ScriptingApi.h"
#include "core/Locator.h"
#include "resources/ResourceManager.h"

namespace Engine
{
    std::unique_ptr<ScriptClass> ScriptSystem::LoadScriptClass(const std::filesystem::path& filePath,
                                                               std::string_view className)
    {
        const auto absoluteFilePath{ResourceManager::GetResourcePath(filePath)};
        const auto result{m_lua.script_file(absoluteFilePath, sol::script_pass_on_error)};
        if (!result.valid()) {
            const sol::error error{result};
            const sol::call_status status{result.status()};
            Locator::GetLogger()->Error("Error loading script {}: {} error\n\t{}", absoluteFilePath.c_str(),
                                        sol::to_string(status), error.what());
            return {};
        }
        const sol::optional<sol::table> maybeScriptClass{m_lua[className]};
        if (!maybeScriptClass) {
            Locator::GetLogger()->Warn("Script '{}' doesn't have '{}' class", filePath.c_str(), className);
            return {};
        }
        sol::table scriptClass{maybeScriptClass.value()};
        const sol::table entityScriptClass{m_lua["EntityScript"]};
        const auto isEntityScript{m_lua["utils"]["instance_of"](scriptClass, entityScriptClass)};
        if (!isEntityScript.get<bool>()) {
            Locator::GetLogger()->Warn("Script '{}' doesn't inherit from EntityScript", className);
            return {};
        }
        sol::table scriptClasses{m_lua["scriptClasses"]};
        scriptClasses[scriptClasses.size()] = scriptClass;
        return std::make_unique<ScriptClass>(className, scriptClass);
    }

    std::optional<ScriptInstance> ScriptSystem::CreateScriptInstance(const std::filesystem::path& filePath,
                                                                     std::string_view className,
                                                                     entt::handle entityHandle)
    {
        StringId scriptClassId{StringId::Intern(filePath.c_str())};
        ScriptClass* scriptClass{GetScriptClass(scriptClassId)};
        if (!scriptClass) {
            if (std::unique_ptr<ScriptClass> newScriptClass{LoadScriptClass(filePath, className)}) {
                StoreScriptClass(scriptClassId, std::move(newScriptClass));
                scriptClass = GetScriptClass(scriptClassId);
            } else {
                return {};
            }
        }
        const sol::optional<sol::function> maybeScriptClassConstructor{scriptClass->GetConstructor()};
        if (!maybeScriptClassConstructor) {
            Locator::GetLogger()->Warn("Script '{}' doesn't have a constructor", scriptClass->GetClassName());
            return {};
        }
        const sol::optional<sol::table> maybeScriptInstance{
            maybeScriptClassConstructor.value()(scriptClass->GetLuaClass())};
        if (!maybeScriptInstance) {
            Locator::GetLogger()->Warn("Failed to instantiate '{}' script", scriptClass->GetClassName());
            return {};
        }
        sol::table scriptInstance{maybeScriptInstance.value()};
        scriptInstance["entity"] = Entity{entityHandle};
        sol::table scriptInstances{m_lua["scriptInstances"]};
        scriptInstances[scriptInstances.size()] = scriptInstance;
        return ScriptInstance{scriptClassId, scriptInstance};
    }

    void ScriptSystem::Init()
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        const std::string packagePath{m_lua["package"]["path"]};
        m_lua["package"]["path"] =
            packagePath + (packagePath.empty() ? "" : ";") + (s_scriptingLibPath / "?.lua").string();
        m_lua.require_file("utils", s_scriptingLibPath / "utils.lua");
        m_lua.script_file(s_scriptingLibPath / "entity_script.lua");
        CreateScriptBindings();
        m_lua.create_named_table("scriptClasses");
        m_lua.create_named_table("scriptInstances");
    }

    void ScriptSystem::ShutDown() { m_scriptClasses.clear(); }

    void ScriptSystem::CreateScriptBindings()
    {
        m_lua.new_usertype<StringId>("StringId", "id", &StringId::GetSid, "str", &StringId::GetString);
        m_lua.new_usertype<Entity>("Entity", "id", sol::property(&Entity::GetId));
        m_lua.new_usertype<glm::vec2>(
            "Vec2", sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(), "x",
            &glm::vec2::x, "y", &glm::vec2::y);
        m_lua.new_usertype<glm::vec3>(
            "Vec3", sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(), "x",
            &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
        m_lua.new_usertype<AABB>("AABB", "center", &AABB::center, "width", &AABB::width, "height",
                                 &AABB::height);
        m_lua.new_usertype<InputValue>("InputValue", "value", &InputValue::value);
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

    void ScriptSystem::StoreScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass)
    {
        m_scriptClasses[scriptId] = std::move(scriptClass);
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
} // namespace Engine
