#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Scene.h"
#include "input/InputHandler.h"
#include "renderer/Renderer2D.h"

#include <filesystem>
#include <memory>

namespace Engine
{
    class SceneLoader
    {
    public:
        SceneLoader() = delete;
        static std::unique_ptr<Scene> Load(const std::filesystem::path& sceneFilepath,
                                           InputHandler& inputHandler, Renderer2D& renderer);
    };
} // namespace Engine

#endif
