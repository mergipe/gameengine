#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "assets/AssetManager.h"
#include "renderer/Renderer.h"
#include <entt/entt.hpp>
#include <filesystem>

namespace Engine
{
    class LevelLoader
    {
    public:
        void loadLevel(const std::filesystem::path& levelFilepath, entt::registry& registry,
                       AssetManager& assetManager, Renderer& renderer);
    };
} // namespace Engine

#endif
