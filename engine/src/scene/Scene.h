#ifndef SCENE_H
#define SCENE_H

#include "SceneContext.h"
#include "Systems.h"

#include <memory>

namespace Engine
{
    class Scene final
    {
    public:
        explicit Scene(SceneContext* sceneContext);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void Update(float timeStep);
        void Render(float frameExtrapolationTimeStep);

    private:
        std::unique_ptr<PhysicsSystem> m_physicsSystem{};
        std::unique_ptr<RenderingSystem> m_renderingSystem{};
        std::unique_ptr<DebugRenderingSystem> m_debugRenderingSystem{};
        std::unique_ptr<SpriteAnimationSystem> m_animationSystem{};
        std::unique_ptr<PlayerInputSystem> m_playerInputSystem{};
        SceneContext* m_sceneContext{};
    };
} // namespace Engine

#endif
