#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "SceneContext.h"

#include <memory>
#include <yaml-cpp/yaml.h>

namespace Engine
{
    struct SceneManagerUIData {
        bool showEntityEditor{};
    };

    class SceneManager
    {
    public:
        void Init();
        void ShutDown();
        void LoadScene(const StringId& sceneId);
        [[nodiscard]] Scene* GetCurrentScene() const { return m_currentScene.get(); }
        void RenderDevGui();

    private:
        void LoadEntities(const YAML::Node& entitiesNode);
        SceneContext m_sceneContext{};
        SceneManagerUIData m_uiData{};
        std::unique_ptr<Scene> m_currentScene{};
    };
} // namespace Engine

#endif
