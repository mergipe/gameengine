#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "Scene.h"
#include "renderer/Renderer2D.h"
#include "resources/ResourceManager.h"
#include <filesystem>
#include <memory>

namespace Engine
{
    class SceneLoader
    {
    public:
        static std::unique_ptr<Scene> load(const std::filesystem::path& sceneFilepath, Renderer2D& renderer,
                                           ResourceManager& resourceManager);
    };
} // namespace Engine

#endif
