#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "SceneData.h"
#include "resources/ResourceManager.h"
#include <entt/entt.hpp>
#include <filesystem>
#include <sol/sol.hpp>

namespace Engine
{
    class SceneLoader
    {
    public:
        SceneData load(sol::state& lua, const std::filesystem::path& sceneFilepath, entt::registry& registry,
                       ResourceManager& resourceManager);
    };
} // namespace Engine

#endif
