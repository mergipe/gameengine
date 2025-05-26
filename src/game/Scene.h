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
        Scene(Renderer2D* renderer, ResourceManager* resourceManager);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void update(float timeStep);
        void render(float frameExtrapolationTimeStep);

    private:
        SceneData m_sceneData{};
        Renderer2D* m_renderer{};
        ResourceManager* m_resourceManager{};
        std::unique_ptr<entt::registry> m_registry{};
        std::unique_ptr<MovementSystem> m_movementSystem{};
        std::unique_ptr<RenderingSystem> m_renderingSystem{};
        std::unique_ptr<DebugRenderingSystem> m_debugRenderingSystem{};
        std::unique_ptr<SpriteAnimationSystem> m_animationSystem{};
        std::unique_ptr<CollisionSystem> m_collisionSystem{};
        std::unique_ptr<PlayerInputSystem> m_playerInputSystem{};
        std::unique_ptr<ScriptingSystem> m_scriptingSystem{};
    };
} // namespace Engine

#endif
