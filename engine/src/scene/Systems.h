#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Entity.h"
#include "ScriptClass.h"
#include "ScriptInstance.h"
#include "core/FileSystem.h"
#include "events/EventBus.h"
#include "input/InputManager.h"

#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <sol/sol.hpp>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class System
    {
    public:
        explicit System(entt::registry* registry)
            : m_registry{registry}
        {
        }
        System(const System&) = delete;
        System(System&&) = delete;
        System& operator=(const System&) = delete;
        System& operator=(System&&) = delete;
        virtual ~System() = default;
        entt::registry& GetRegistry() const { return *m_registry; }

    private:
        entt::registry* m_registry{};
    };

    class PhysicsSystem final : public System
    {
    public:
        explicit PhysicsSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void Start();
        void Update(float timeStep);
    };

    class RenderingSystem final : public System
    {
    public:
        explicit RenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void Update(float frameExtrapolationTimeStep);
    };

    class SpriteAnimationSystem final : public System
    {
    public:
        explicit SpriteAnimationSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void Update();
    };

    class PlayerInputSystem final : public System
    {
    public:
        explicit PlayerInputSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void Start();
        void SubscribeToEvents(EventBus& eventBus);
        void OnInputCommand(const InputEvent& event) const;
    };

    class ScriptingSystem final
        : public System // TODO: move this to a separate file (maybe change name to ScriptEngine)
    {
    public:
        explicit ScriptingSystem(entt::registry* registry);
        ScriptingSystem(const ScriptingSystem&) = delete;
        ScriptingSystem(ScriptingSystem&&) = delete;
        ScriptingSystem& operator=(const ScriptingSystem&) = delete;
        ScriptingSystem& operator=(ScriptingSystem&&) = delete;
        ~ScriptingSystem() override;
        std::optional<ScriptInstance> CreateScriptInstance(const std::filesystem::path& filePath,
                                                           std::string_view className, entt::entity entity);
        void Start();
        void Update(float timeStep);
        void ShutDown();

    private:
        static inline const std::filesystem::path s_scriptingLibPath{FileSystem::GetAbsolutePath("lua")};
        std::unique_ptr<ScriptClass> LoadScriptClass(const std::filesystem::path& filePath,
                                                     std::string_view className);
        void CreateScriptBindings();
        void StoreScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass);
        sol::object GetComponent(Entity entity, const sol::table& type);
        ScriptClass* GetScriptClass(const StringId& scriptId) const;
        std::unordered_map<StringId, std::unique_ptr<ScriptClass>> m_scriptClasses{};
        std::unordered_map<const void*, std::function<sol::object(Entity)>> m_componentTypes{};
        sol::state m_lua{};
    };
} // namespace Engine

#endif
