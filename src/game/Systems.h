#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ScriptClass.h"
#include "ScriptInstance.h"
#include "events/EventBus.h"
#include "input/InputHandler.h"
#include "renderer/Renderer2D.h"
#include <entt/entt.hpp>
#include <filesystem>
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
        entt::registry& getRegistry() const { return *m_registry; }

    private:
        entt::registry* m_registry{};
    };

    class MovementSystem final : public System
    {
    public:
        explicit MovementSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(float timeStep);
    };

    class RenderingSystem final : public System
    {
    public:
        explicit RenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer2D& renderer, float frameExtrapolationTimeStep);
    };

    class DebugRenderingSystem final : public System
    {
    public:
        explicit DebugRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer2D& renderer, float frameExtrapolationTimeStep);
    };

    class SpriteAnimationSystem final : public System
    {
    public:
        explicit SpriteAnimationSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update();
    };

    class CollisionSystem final : public System
    {
    public:
        explicit CollisionSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update();
    };

    class PlayerInputSystem final : public System
    {
    public:
        explicit PlayerInputSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void start(InputHandler& inputHandler);
        void subscribeToEvents(EventBus& eventBus);
        void onInputCommand(const InputEvent& event) const;
    };

    class ScriptingSystem final : public System
    {
    public:
        explicit ScriptingSystem(entt::registry* registry);
        ScriptingSystem(const ScriptingSystem&) = delete;
        ScriptingSystem(ScriptingSystem&&) = delete;
        ScriptingSystem& operator=(const ScriptingSystem&) = delete;
        ScriptingSystem& operator=(ScriptingSystem&&) = delete;
        ~ScriptingSystem() override;
        std::optional<ScriptInstance> createScriptInstance(const std::filesystem::path& filepath,
                                                           std::string_view className, entt::entity entity);
        void start();
        void update(float timeStep);

    private:
        std::unique_ptr<ScriptClass> loadScriptClass(const std::filesystem::path& filepath,
                                                     std::string_view className);
        void createScriptBindings();
        void storeScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass);
        ScriptClass* getScriptClass(const StringId& scriptId) const;
        std::unordered_map<StringId, std::unique_ptr<ScriptClass>> m_scriptClasses{};
        sol::state m_lua{};
    };
} // namespace Engine

#endif
