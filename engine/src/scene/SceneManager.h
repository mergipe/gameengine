#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "core/Yaml.h"

#include <memory>
#include <entt/entity/registry.hpp>

namespace Engine
{
    struct SceneManagerDebugUIData {
        bool showEntityEditor{false};
    };

    class SceneManager
    {
    public:
        void Init();
        void ShutDown();
        void LoadScene(const StringId& sceneId);
        [[nodiscard]] Scene* GetCurrentScene() const { return m_currentScene.get(); }
        void RenderDevGui();
        void OnViewportResize(int width, int height);

    private:
        void LoadEntities(const YAML::Node& entitiesNode);

        entt::registry m_ecsRegistry{};
        std::unique_ptr<Scene> m_currentScene{};
        SceneManagerDebugUIData m_debugUIData{};
    };
} // namespace Engine

#endif
