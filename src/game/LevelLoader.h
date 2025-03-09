#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "assets/AssetManager.h"
#include "renderer/Renderer.h"
#include <entt/entt.hpp>
#include <filesystem>
#include <sol/sol.hpp>

namespace Engine
{
    struct TilemapData {
        float width;
        float height;
    };

    struct LevelData {
        TilemapData mapData;
    };

    class LevelLoader
    {
    public:
        LevelData loadLevel(sol::state& luaState, const std::filesystem::path& levelFilepath,
                            entt::registry& registry, AssetManager& assetManager, Renderer& renderer);
    };
} // namespace Engine

#endif
