#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "resources/ResourceManager.h"
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
                            entt::registry& registry, ResourceManager& resourceManager);
    };
} // namespace Engine

#endif
