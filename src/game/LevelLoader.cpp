#include "LevelLoader.h"
#include "assets/AssetType.h"
#include "core/Logger.h"
#include <sol/sol.hpp>

namespace Engine
{
    void loadAssets(sol::table& level, AssetManager& assetManager, Renderer& renderer)
    {
        sol::table assets{level["assets"]};
        for (const auto& entry : assets) {
            sol::table asset{entry.second};
            std::string typeStr{asset["type"]};
            auto type{getAssetType(typeStr)};
            if (!type) {
                Logger::error("Asset type {} unknown", typeStr);
                continue;
            }
            std::string assetId{asset["id"]};
            std::string filepath{asset["file"]};
            switch (type.value()) {
            case AssetType::texture:
                assetManager.addTexture(assetId, renderer.loadTexture(filepath));
                break;
            case AssetType::font:
                assetManager.addFont(assetId, filepath, asset["font_size"]);
                break;
            }
        }
    }

    void loadTilemap([[maybe_unused]] sol::table& tilemap) {}

    void loadEntities([[maybe_unused]] sol::table& entities, [[maybe_unused]] entt::registry& registry) {}

    void LevelLoader::loadLevel(const std::filesystem::path& levelFilepath, entt::registry& registry,
                                AssetManager& assetManager, Renderer& renderer)
    {
        sol::state luaState{};
        luaState.open_libraries(sol::lib::base);
        sol::load_result loadResult{luaState.load_file(levelFilepath)};
        if (!loadResult.valid()) {
            sol::error error{loadResult};
            Logger::error("Error loading lua script: {}", error.what());
            return;
        }
        loadResult();
        sol::table level{luaState["Level"]};
        loadAssets(level, assetManager, renderer);
        sol::table tilemap{luaState["tilemap"]};
        loadTilemap(tilemap);
        sol::table entities{luaState["entities"]};
        loadEntities(entities, registry);
    }
} // namespace Engine
