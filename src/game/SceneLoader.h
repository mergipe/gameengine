#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "SceneData.h"
#include "Systems.h"
#include "resources/ResourceManager.h"
#include <entt/entt.hpp>
#include <filesystem>

namespace Engine
{
    class SceneLoader
    {
    public:
        SceneData load(const std::filesystem::path& sceneFilepath, entt::registry& registry,
                       ResourceManager& resourceManager, ScriptingSystem& scriptingSystem);
    };
} // namespace Engine

#endif
