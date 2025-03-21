#ifndef SCENE_H
#define SCENE_H

#include "SceneData.h"
#include "Systems.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include <entt/entt.hpp>
#include <memory>
#include <sol/sol.hpp>

namespace Engine
{
    class Scene final
    {
    public:
        Scene(Renderer2D* renderer, ResourceManager* resourceManager, int windowWidth, int windowHeight);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void update(float timeStep);
        void render(float frameExtrapolationTimeStep);

    private:
        sol::state m_luaState{};
        SceneData m_sceneData{};
        Renderer2D* m_renderer{};
        ResourceManager* m_resourceManager{};
        std::unique_ptr<entt::registry> m_registry{};
        std::unique_ptr<MovementSystem> m_movementSystem{};
        std::unique_ptr<SpriteRenderingSystem> m_spriteRenderingSystem{};
        std::unique_ptr<SpriteAnimationSystem> m_animationSystem{};
        std::unique_ptr<CollisionSystem> m_collisionSystem{};
        std::unique_ptr<PlayerInputSystem> m_playerInputSystem{};
        std::unique_ptr<LifecycleSystem> m_lifecycleSystem{};
        std::unique_ptr<CameraMovementSystem> m_cameraMovementSystem{};
        std::unique_ptr<BoxColliderRenderingSystem> m_boxColliderRenderingSystem{};
        std::unique_ptr<ScriptSystem> m_scriptSystem{};
    };
} // namespace Engine

#endif
