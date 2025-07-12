#ifndef SCENE_H
#define SCENE_H

#include "SceneData.h"
#include "Systems.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include <entt/entt.hpp>
#include <memory>

namespace Engine
{
    class Scene final
    {
    public:
        Scene(Renderer2D* renderer, ResourceManager* resourceManager,
              std::unique_ptr<entt::registry> registry, std::unique_ptr<ScriptingSystem> scriptingSystem,
              const SceneData& sceneData);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void update(float timeStep);
        void render(float frameExtrapolationTimeStep);

    private:
        Renderer2D* m_renderer{};
        ResourceManager* m_resourceManager{};
        std::unique_ptr<entt::registry> m_registry{};
        std::unique_ptr<ScriptingSystem> m_scriptingSystem{};
        std::unique_ptr<MovementSystem> m_movementSystem{};
        std::unique_ptr<RenderingSystem> m_renderingSystem{};
        std::unique_ptr<DebugRenderingSystem> m_debugRenderingSystem{};
        std::unique_ptr<SpriteAnimationSystem> m_animationSystem{};
        std::unique_ptr<CollisionSystem> m_collisionSystem{};
        std::unique_ptr<PlayerInputSystem> m_playerInputSystem{};
        SceneData m_sceneData{};
    };
} // namespace Engine

#endif
