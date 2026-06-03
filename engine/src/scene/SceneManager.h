#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "SceneContext.h"
#include "core/Yaml.h"

#include <memory>

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
        SceneContext m_sceneContext{};
        SceneManagerDebugUIData m_debugUIData{};
        std::unique_ptr<Scene> m_currentScene{};
    };
} // namespace Engine

#endif
