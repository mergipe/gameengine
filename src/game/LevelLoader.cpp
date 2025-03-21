#include "LevelLoader.h"
#include "Components.h"
#include "core/IO.h"
#include "core/Logger.h"
#include "resources/ResourceType.h"
#include "resources/Texture2D.h"
#include <fstream>
#include <sol/sol.hpp>

namespace Engine
{
    TextureConfig getTextureConfig(const sol::table& texture)
    {
        TextureConfig config{};
        const std::optional<TextureFiltering> minFilter{
            getTextureFiltering(texture["min_filter"].get_or(std::string{}))};
        if (minFilter) {
            config.minFilter = *minFilter;
        }
        const std::optional<TextureFiltering> magFilter{
            getTextureFiltering(texture["mag_filter"].get_or(std::string{}))};
        if (magFilter) {
            config.magFilter = *magFilter;
        }
        const std::optional<TextureWrapping> wrapX{
            getTextureWrapping(texture["wrap_x"].get_or(std::string()))};
        if (wrapX) {
            config.wrapX = *wrapX;
        }
        const std::optional<TextureWrapping> wrapY{
            getTextureWrapping(texture["wrap_y"].get_or(std::string()))};
        if (wrapY) {
            config.wrapY = *wrapY;
        }
        return config;
    }

    void loadAssets(const sol::table& level, ResourceManager& resourceManager)
    {
        const sol::table assets{level["assets"]};
        for (const auto& entry : assets) {
            const sol::table asset{entry.second};
            const std::string typeStr{asset["type"]};
            const auto type{getResourceType(typeStr)};
            if (!type) {
                Logger::error("Asset type {} unknown", typeStr);
                continue;
            }
            const std::string assetId{asset["id"]};
            const std::string filepath{asset["file"]};
            switch (type.value()) {
            case ResourceType::texture:
                resourceManager.loadTexture(assetId, filepath, getTextureConfig(asset));
                break;
            default:
                break;
            }
        }
    }

    TilemapData loadTilemap(const sol::table& level, entt::registry& registry)
    {
        const sol::table tilemap{level["tilemap"]};
        const std::string mapFilepath{tilemap["map_file"]};
        Logger::info("Loading map from {}", mapFilepath);
        const std::string textureId{tilemap["texture_id"]};
        const int mapRows{tilemap["map_rows"]};
        const int mapCols{tilemap["map_cols"]};
        const int tilesetCols{tilemap["tileset_cols"]};
        const float tileSize{tilemap["tile_size"]};
        const float scale{tilemap["scale"]};
        std::ifstream tilemapFile{mapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} file for reading", mapFilepath);
        }
        const std::vector<std::vector<int>> tilemapCsv{IO::parseIntCsvFile(tilemapFile)};
        tilemapFile.close();
        for (size_t i{0}; i < tilemapCsv.size(); ++i) {
            const std::vector<int> values{tilemapCsv[i]};
            for (size_t j{0}; j < values.size(); ++j) {
                auto tile{registry.create()};
                registry.emplace<TagComponent>(tile, "tile");
                registry.emplace<TransformComponent>(tile,
                                                     glm::vec3{static_cast<float>(j) * tileSize * scale,
                                                               static_cast<float>(i) * tileSize * scale,
                                                               0.0f},
                                                     glm::vec3{scale, scale, 0.0f});
                const int tileId{values[j]};
                registry.emplace<SpriteComponent>(
                    tile, textureId,
                    Rect{glm::vec2{tileSize * static_cast<float>(tileId % tilesetCols),
                                   tileSize * static_cast<float>(tileId / tilesetCols)},
                         tileSize, tileSize},
                    0, false);
            }
        }
        return TilemapData{static_cast<float>(mapCols) * tileSize * scale,
                           static_cast<float>(mapRows) * tileSize * scale};
    }

    void loadEntities(const sol::table& level, entt::registry& registry)
    {
        const sol::table entities{level["entities"]};
        for (const auto& entry : entities) {
            const sol::table entityTable{entry.second};
            auto entity{registry.create()};
            const sol::optional<sol::table> maybeComponents{entityTable["components"]};
            if (maybeComponents) {
                const sol::table components{maybeComponents.value()};
                const sol::optional<sol::table> maybeTag{components["tag"]};
                if (maybeTag) {
                    const sol::table tag{maybeTag.value()};
                    registry.emplace<TagComponent>(entity, tag["name"]);
                }
                const sol::optional<sol::table> maybeTransform{components["transform"]};
                if (maybeTransform) {
                    const sol::table transform{maybeTransform.value()};
                    registry.emplace<TransformComponent>(
                        entity,
                        glm::vec3{transform["position"]["x"], transform["position"]["y"],
                                  transform["position"]["z"].get_or(0.0f)},
                        glm::vec3{transform["scale"]["x"].get_or(1.0f), transform["scale"]["y"].get_or(1.0f),
                                  transform["scale"]["z"].get_or(1.0f)},
                        glm::vec3{transform["rotation"]["x"].get_or(0.0f),
                                  transform["rotation"]["y"].get_or(0.0f),
                                  transform["rotation"]["z"].get_or(0.0f)});
                }
                const sol::optional<sol::table> maybeRigidBody{components["rigid_body"]};
                if (maybeRigidBody) {
                    const sol::table rigid_body{maybeRigidBody.value()};
                    registry.emplace<RigidBody2DComponent>(
                        entity, glm::vec3{rigid_body["velocity"]["x"].get_or(0.0f),
                                          rigid_body["velocity"]["y"].get_or(0.0f),
                                          rigid_body["velocity"]["z"].get_or(0.0f)});
                }
                const sol::optional<sol::table> maybeSprite{components["sprite"]};
                if (maybeSprite) {
                    const sol::table sprite{maybeSprite.value()};
                    registry.emplace<SpriteComponent>(entity, std::string{sprite["texture_id"]},
                                                      Rect{glm::vec2{sprite["source_rect_x"].get_or(0.0f),
                                                                     sprite["source_rect_y"].get_or(0.0f)},
                                                           sprite["width"], sprite["height"]},
                                                      sprite["z_index"].get_or(1),
                                                      sprite["has_fixed_position"].get_or(false));
                }
                const sol::optional<sol::table> maybeAnimation{components["animation"]};
                if (maybeAnimation) {
                    const sol::table animation{maybeAnimation.value()};
                    registry.emplace<SpriteAnimationComponent>(entity, animation["frames_count"].get_or(1),
                                                               animation["frames_per_second"].get_or(1));
                }
                const sol::optional<sol::table> maybeBoxCollider{components["box_collider"]};
                if (maybeBoxCollider) {
                    const sol::table boxCollider{maybeBoxCollider.value()};
                    registry.emplace<BoxCollider2DComponent>(
                        entity, boxCollider["width"], boxCollider["height"],
                        glm::vec2{boxCollider["offset"]["x"].get_or(0.0f),
                                  boxCollider["offset"]["y"].get_or(0.0f)});
                }
                if (components["player_input"]) {
                    registry.emplace<PlayerInputComponent>(entity);
                }
                if (components["camera_follow"]) {
                    registry.emplace<CameraFollowComponent>(entity);
                }
                const sol::optional<sol::table> maybeScript{components["script"]};
                if (maybeScript) {
                    const sol::table script{maybeScript.value()};
                    const sol::function func{script["on_update"]};
                    registry.emplace<ScriptComponent>(entity, func);
                }
            }
        }
    }

    LevelData LevelLoader::loadLevel(sol::state& luaState, const std::filesystem::path& levelFilepath,
                                     entt::registry& registry, ResourceManager& resourceManager)
    {
        sol::load_result loadResult{luaState.load_file(levelFilepath)};
        if (!loadResult.valid()) {
            sol::error error{loadResult};
            Logger::error("Error loading lua script: {}", error.what());
            return {};
        }
        loadResult();
        const sol::table level{luaState["Level"]};
        loadAssets(level, resourceManager);
        auto mapData{loadTilemap(level, registry)};
        loadEntities(level, registry);
        return LevelData{mapData};
    }
} // namespace Engine
