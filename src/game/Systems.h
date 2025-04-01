#ifndef SYSTEMS_H
#define SYSTEMS_H

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

    class ScriptSystem final : public System
    {
    public:
        ScriptSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(float timeStep);
        void createScriptBindings(sol::state& lua);

    private:
        glm::vec3 getEntityPosition(entt::entity entity);
        glm::vec3 getEntityVelocity(entt::entity entity);
        void setEntityPosition(entt::entity entity, glm::vec3 position);
        void setEntityVelocity(entt::entity entity, glm::vec3 velocity);
        void setEntityRotation(entt::entity entity, glm::vec3 rotation);
    };
} // namespace Engine

#endif
