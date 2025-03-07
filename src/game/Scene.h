#ifndef SCENE_H
#define SCENE_H

#include "SceneData.h"
#include "Systems.h"
#include "assets/AssetManager.h"
#include "renderer/Renderer.h"
#include <entt/entt.hpp>
#include <memory>
#include <string_view>

namespace Engine
{
    class Scene final
    {
    public:
        Scene(Renderer* renderer, AssetManager* assetManager, int windowWidth, int windowHeight);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void update(float timeStep);
        void render(float frameExtrapolationTimeStep);

    private:
        void loadMap(std::string_view tilemapFilename, float tileWidth, float tileHeight, int tilesetColumns,
                     float scale);
        void loadEntities();
        SceneData m_sceneData{};
        Renderer* m_renderer{};
        AssetManager* m_assetManager{};
        std::unique_ptr<entt::registry> m_registry{};
        std::unique_ptr<MovementSystem> m_movementSystem{};
        std::unique_ptr<SpriteRenderingSystem> m_spriteRenderingSystem{};
        std::unique_ptr<AnimationSystem> m_animationSystem{};
        std::unique_ptr<CollisionSystem> m_collisionSystem{};
        std::unique_ptr<DamageSystem> m_damageSystem{};
        std::unique_ptr<PlayerInputSystem> m_playerInputSystem{};
        std::unique_ptr<ProjectileEmitSystem> m_projectileEmitSystem{};
        std::unique_ptr<LifecycleSystem> m_lifecycleSystem{};
        std::unique_ptr<CameraMovementSystem> m_cameraMovementSystem{};
        std::unique_ptr<HealthBarRenderingSystem> m_healthBarRenderingSystem{};
        std::unique_ptr<BoxColliderRenderingSystem> m_boxColliderRenderingSystem{};
        std::unique_ptr<TextRenderingSystem> m_textRenderingSystem{};
    };
} // namespace Engine

#endif
