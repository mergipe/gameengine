#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Script.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace Engine
{
    class System
    {
    public:
        System(entt::registry* registry)
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
        MovementSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(float timeStep);
    };

    class RenderingSystem final : public System
    {
    public:
        RenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer2D& renderer, const ResourceManager& resourceManager,
                    float frameExtrapolationTimeStep);
    };

    class DebugRenderingSystem final : public System
    {
    public:
        DebugRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer2D& renderer, float frameExtrapolationTimeStep);
    };

    class SpriteAnimationSystem final : public System
    {
    public:
        SpriteAnimationSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update();
    };

    class CollisionSystem final : public System
    {
    public:
        CollisionSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update();
    };

    class PlayerInputSystem final : public System
    {
    public:
        PlayerInputSystem(entt::registry* registry)
            : System{registry}
        {
        }
    };

    class ScriptingSystem final : public System
    {
    public:
        ScriptingSystem(entt::registry* registry);
        std::optional<Script> loadScript(entt::entity entity, const std::filesystem::path& filepath,
                                         std::string_view className);
        void start();
        void update(float timeStep);

    private:
        void createScriptBindings();
        sol::state m_lua{};
    };
} // namespace Engine

#endif
