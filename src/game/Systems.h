#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "assets/AssetManager.h"
#include "events/EventBus.h"
#include "game/SceneData.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

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
        void update(float timeStep, const SceneData& sceneData);
    };

    class SpriteRenderingSystem final : public System
    {
    public:
        SpriteRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer& renderer, const AssetManager& assetManager, const Camera& camera,
                    float frameExtrapolationTimeStep);
    };

    class AnimationSystem final : public System
    {
    public:
        AnimationSystem(entt::registry* registry)
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

    class BoxColliderRenderingSystem final : public System
    {
    public:
        BoxColliderRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer& renderer, const Camera& camera, float frameExtrapolationTimeStep);
    };

    class DamageSystem final : public System
    {
    public:
        DamageSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void subscribeToEvents(EventBus& eventBus);
        void onCollision(CollisionEvent& event);

    private:
        void onProjectileHitsEntity(entt::entity& projectile, entt::entity& entity);
    };

    class PlayerInputSystem final : public System
    {
    public:
        PlayerInputSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void subscribeToEvents(EventBus& eventBus);
        void onKeyPressed(KeyPressedEvent& event);

    private:
        void move(const entt::entity& entity, glm::vec2 velocity, int spriteIndex);
    };

    class CameraMovementSystem final : public System
    {
    public:
        CameraMovementSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(SceneData& sceneData);
    };

    // this and ProjectileEmmiterComponent should become a script in the future
    class ProjectileEmitSystem final : public System
    {
    public:
        ProjectileEmitSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void subscribeToEvents(EventBus& eventBus);
        void update();
        void onEmitProjectile(ProjectileEmitEvent& event);

    private:
        void emitProjectile(const entt::entity& entity, glm::vec2 velocity, int duration, int damage,
                            bool isFriendly);
    };

    class LifecycleSystem final : public System
    {
    public:
        LifecycleSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update();
    };

    class TextRenderingSystem final : public System
    {
    public:
        TextRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer& renderer, const AssetManager& assetManager, const Camera& camera);
    };

    class HealthBarRenderingSystem final : public System
    {
    public:
        HealthBarRenderingSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(Renderer& renderer, const Font& font, const Camera& camera);
    };

    class ScriptSystem final : public System
    {
    public:
        ScriptSystem(entt::registry* registry)
            : System{registry}
        {
        }
        void update(float timeStep);
        void createScriptBindings(sol::state& luaState);

    private:
        glm::vec2 getEntityPosition(entt::entity entity);
        glm::vec2 getEntityVelocity(entt::entity entity);
        void setEntityPosition(entt::entity entity, glm::vec2 position);
        void setEntityVelocity(entt::entity entity, glm::vec2 velocity);
        void setEntityRotation(entt::entity entity, float rotation);
        void setProjectileVelocity(entt::entity entity, glm::vec2 velocity);
    };
} // namespace Engine

#endif
