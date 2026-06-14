#include "ScriptSystem.h"

#include "ScriptInstance.h"
#include "api/Core.h"
#include "api/Physics.h"
#include "core/Locator.h"
#include "resources/ResourceManager.h"
#include "scene/Components.h"

namespace Engine
{
    std::optional<ScriptingApi::Entity> CreateEntity(const StringId& entityTemplateId)
    {
        const auto entityTemplate{Locator::GetResourceManager()->GetEntityTemplate(entityTemplateId)};
        if (!entityTemplate) {
            Locator::GetLogger()->Error("Entity template {} not found", entityTemplateId.GetString());
            return {};
        }
        auto& entity{Locator::GetSceneManager()->GetCurrentScene()->CreateEntity(entityTemplate.value())};
        return ScriptingApi::Entity{&entity};
    }

    void ScriptSystem::Init()
    {
        m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        AppendPackagePath((s_scriptingLibPath / "?.lua").string());
        m_lua.require_file("utils", s_scriptingLibPath / "utils.lua");
        m_lua.script_file(s_scriptingLibPath / "entity_script.lua");
        SetBindings();
        Locator::GetLogger()->Info("Script system initialized");
    }

    void ScriptSystem::ShutDown()
    {
        m_scriptClasses.clear();
        Locator::GetLogger()->Info("Script system shutdown");
    }

    std::optional<ScriptInstance> ScriptSystem::CreateScriptInstance(const ScriptData& scriptData,
                                                                     Entity& entity)
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
        ScriptInstance scriptInstance{ScriptingApi::Entity{&entity}, scriptClass,
                                      maybeScriptInstance.value()};
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
        BindComponentTypes();
        BindPhysicsTypes();
        SetComponentOperations();
        m_lua.new_usertype<InputValue>("InputValue", "value", &InputValue::value);
    }

    void ScriptSystem::BindCoreTypes()
    {
        auto vec2{m_lua.new_usertype<glm::vec2>(
            "Vec2", sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>())};
        vec2["x"] = &glm::vec2::x;
        vec2["y"] = &glm::vec2::y;
        BindVectorFunctions(vec2);
        auto vec3{m_lua.new_usertype<glm::vec3>(
            "Vec3", sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>())};
        vec3["x"] = &glm::vec3::x;
        vec3["y"] = &glm::vec3::y;
        vec3["z"] = &glm::vec3::z;
        BindVectorFunctions(vec3);
        auto vec4{m_lua.new_usertype<glm::vec4>(
            "Vec4",
            sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>())};
        vec4["x"] = &glm::vec4::x;
        vec4["y"] = &glm::vec4::y;
        vec4["z"] = &glm::vec4::z;
        vec4["w"] = &glm::vec4::w;
        BindVectorFunctions(vec4);
        auto stringId{
            m_lua.new_usertype<StringId>("StringId", sol::constructors<StringId(), StringId(const char*)>())};
        stringId["id"] = sol::property(&StringId::GetId);
        stringId["str"] = sol::property(sol::resolve<std::string_view() const>(&StringId::GetString));
        auto debug{m_lua.new_usertype<ScriptingApi::Debug>("Debug")};
        debug["Log"] = &ScriptingApi::Debug::Log;
        auto timer{m_lua.new_usertype<ScriptingApi::Timer>("Timer")};
        timer["GetMilliseconds"] = &ScriptingApi::Timer::GetMilliseconds;
    }

    void ScriptSystem::BindComponentTypes()
    {
        auto entity{m_lua.new_usertype<ScriptingApi::Entity>("Entity")};
        entity["id"] = sol::property(&ScriptingApi::Entity::GetId);
        entity["tag"] = sol::property(&ScriptingApi::Entity::GetTag);
        entity["GetComponent"] = [this](ScriptingApi::Entity& entity, const sol::table& componentType) {
            return GetComponent(entity, componentType);
        };
        entity["AddComponent"] = [this](ScriptingApi::Entity& entity, const sol::table& componentType) {
            return AddComponent(entity, componentType);
        };
        entity["RemoveComponent"] = [this](ScriptingApi::Entity& entity, const sol::table& componentType) {
            RemoveComponent(entity, componentType);
        };
        entity["Destroy"] = [](ScriptingApi::Entity& entity) {
            Locator::GetSceneManager()->GetCurrentScene()->DestroyEntity(*entity.GetEntityPtr());
        };
        entity["Create"] = &CreateEntity;
        auto component{m_lua.new_usertype<ScriptingApi::Component>("Component")};
        component["entity"] = sol::property(&ScriptingApi::Component::GetEntity);
        auto transform{m_lua.new_usertype<ScriptingApi::Transform>("Transform", sol::base_classes,
                                                                   sol::bases<ScriptingApi::Component>())};
        transform["position"] =
            sol::property(&ScriptingApi::Transform::GetPosition, &ScriptingApi::Transform::SetPosition);
        transform["rotation"] =
            sol::property(&ScriptingApi::Transform::GetRotation, &ScriptingApi::Transform::SetRotation);
        transform["scale"] =
            sol::property(&ScriptingApi::Transform::GetScale, &ScriptingApi::Transform::SetScale);
        transform["up"] = sol::property(&ScriptingApi::Transform::GetUp);
        transform["right"] = sol::property(&ScriptingApi::Transform::GetRight);
        transform["forward"] = sol::property(&ScriptingApi::Transform::GetForward);
        auto rigidBody2D{m_lua.new_usertype<ScriptingApi::RigidBody2D>(
            "RigidBody2D", sol::base_classes, sol::bases<ScriptingApi::Component>())};
        rigidBody2D["linearVelocity"] = sol::property(&ScriptingApi::RigidBody2D::GetLinearVelocity,
                                                      &ScriptingApi::RigidBody2D::SetLinearVelocity);
        rigidBody2D["angularVelocity"] = sol::property(&ScriptingApi::RigidBody2D::GetAngularVelocity,
                                                       &ScriptingApi::RigidBody2D::SetAngularVelocity);
        rigidBody2D["ApplyForce"] = &ScriptingApi::RigidBody2D::ApplyForce;
        rigidBody2D["ApplyTorque"] = &ScriptingApi::RigidBody2D::ApplyTorque;
        auto collider2D{m_lua.new_usertype<ScriptingApi::Collider2D>("Collider2D", sol::base_classes,
                                                                     sol::bases<ScriptingApi::Component>())};
    }

    void ScriptSystem::BindPhysicsTypes()
    {
        auto collisionPoint2D{m_lua.new_usertype<ScriptingApi::CollisionPoint2D>("CollisionPoint2D")};
        collisionPoint2D["anchorA"] = &ScriptingApi::CollisionPoint2D::anchorA;
        collisionPoint2D["anchorB"] = &ScriptingApi::CollisionPoint2D::anchorB;
        collisionPoint2D["normalImpulse"] = &ScriptingApi::CollisionPoint2D::normalImpulse;
        collisionPoint2D["normalVelocity"] = &ScriptingApi::CollisionPoint2D::normalVelocity;
        collisionPoint2D["separation"] = &ScriptingApi::CollisionPoint2D::separation;
        collisionPoint2D["tangentImpulse"] = &ScriptingApi::CollisionPoint2D::tangentImpulse;
        collisionPoint2D["totalNormalImpulse"] = &ScriptingApi::CollisionPoint2D::totalNormalImpulse;
        collisionPoint2D["id"] = &ScriptingApi::CollisionPoint2D::id;
        collisionPoint2D["persisted"] = &ScriptingApi::CollisionPoint2D::persisted;
        auto collision2DData{m_lua.new_usertype<ScriptingApi::Collision2DData>("Collision2DData")};
        collision2DData["points"] = &ScriptingApi::Collision2DData::points;
        collision2DData["normal"] = &ScriptingApi::Collision2DData::normal;
        collision2DData["pointCount"] = &ScriptingApi::Collision2DData::pointCount;
    }

    void ScriptSystem::SetComponentOperations()
    {
        m_componentOperations.emplace(
            sol::table{m_lua["Transform"]}.pointer(),
            CreateComponentOperations<TransformComponent, ScriptingApi::Transform>());
        m_componentOperations.emplace(
            sol::table{m_lua["RigidBody2D"]}.pointer(),
            CreateComponentOperations<RigidBody2DComponent, ScriptingApi::RigidBody2D>());
    }

    sol::table ScriptSystem::GetComponent(ScriptingApi::Entity& entity, const sol::table& componentType)
    {
        if (m_componentOperations.contains(componentType.pointer())) {
            return m_componentOperations[componentType.pointer()].get(*entity.GetEntityPtr());
        }
        return sol::nil;
    }

    sol::table ScriptSystem::AddComponent(ScriptingApi::Entity& entity, const sol::table& componentType)
    {
        if (m_componentOperations.contains(componentType.pointer())) {
            return m_componentOperations[componentType.pointer()].add(*entity.GetEntityPtr());
        }
        return sol::nil;
    }

    void ScriptSystem::RemoveComponent(ScriptingApi::Entity& entity, const sol::table& componentType)
    {
        if (m_componentOperations.contains(componentType.pointer())) {
            m_componentOperations[componentType.pointer()].remove(*entity.GetEntityPtr());
        }
    }
} // namespace Engine
