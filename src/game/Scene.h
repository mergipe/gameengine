#ifndef SCENE_H
#define SCENE_H

#include "ECS.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"
#include <memory>
#include <string_view>

namespace Engine
{
    struct SceneData {
        Camera camera;
        int mapWidth;
        int mapHeight;
    };

    class Scene final
    {
    public:
        Scene(Renderer* renderer, ResourceManager* resourceManager, int windowWidth, int windowHeight);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();
        void update(float timeStep);
        void render(float frameExtrapolationTimeStep);

    private:
        void loadMap(std::string_view tilesetFilename, std::string_view tilemapFilename, int tileWidth,
                     int tileHeight, int tilesetColumns, float scale);
        void loadEntities();
        void loadLevel(int level);
        std::unique_ptr<Registry> m_registry{};
        Renderer* m_renderer{};
        ResourceManager* m_resourceManager{};
        SceneData m_sceneData{};
    };
} // namespace Engine

#endif
