#include "LevelLoader.h"
#include "Components.h"
#include "assets/AssetType.h"
#include "core/IO.h"
#include "core/Logger.h"
#include <fstream>
#include <sol/sol.hpp>

namespace Engine
{
    void loadAssets(const sol::table& level, AssetManager& assetManager, Renderer& renderer)
    {
        const sol::table assets{level["assets"]};
        for (const auto& entry : assets) {
            const sol::table asset{entry.second};
            const std::string typeStr{asset["type"]};
            const auto type{getAssetType(typeStr)};
            if (!type) {
                Logger::error("Asset type {} unknown", typeStr);
                continue;
            }
            const std::string assetId{asset["id"]};
            const std::string filepath{asset["file"]};
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
                registry.emplace<Tile>(tile);
                registry.emplace<TransformComponent>(tile,
                                                     glm::vec2{static_cast<float>(j) * tileSize * scale,
                                                               static_cast<float>(i) * tileSize * scale},
                                                     glm::vec2{scale, scale});
                const int tileId{values[j]};
                registry.emplace<SpriteComponent>(tile, textureId, tileSize, tileSize, 0, false,
                                                  tileSize * static_cast<float>(tileId % tilesetCols),
                                                  tileSize * static_cast<float>(tileId / tilesetCols));
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
                const sol::optional<sol::table> maybeTransform{components["transform"]};
                if (maybeTransform) {
                    const sol::table transform{maybeTransform.value()};
                    registry.emplace<TransformComponent>(
                        entity, glm::vec2{transform["position"]["x"], transform["position"]["y"]},
                        glm::vec2{transform["scale"]["x"].get_or(1.0f), transform["scale"]["y"].get_or(1.0f)},
                        transform["rotation"].get_or(0.0f));
                }
                const sol::optional<sol::table> maybeRigidBody{components["rigid_body"]};
                if (maybeRigidBody) {
                    const sol::table rigid_body{maybeRigidBody.value()};
                    registry.emplace<RigidBodyComponent>(entity,
                                                         glm::vec2{rigid_body["velocity"]["x"].get_or(0.0f),
                                                                   rigid_body["velocity"]["y"].get_or(0.0f)});
                }
                const sol::optional<sol::table> maybeSprite{components["sprite"]};
                if (maybeSprite) {
                    const sol::table sprite{maybeSprite.value()};
                    registry.emplace<SpriteComponent>(
                        entity, std::string{sprite["texture_id"]}, sprite["width"], sprite["height"],
                        sprite["z_index"].get_or(1), sprite["has_fixed_position"].get_or(false),
                        sprite["source_rect_x"].get_or(0.0f), sprite["source_rect_y"].get_or(0.0f));
                }
                const sol::optional<sol::table> maybeAnimation{components["animation"]};
                if (maybeAnimation) {
                    const sol::table animation{maybeAnimation.value()};
                    registry.emplace<AnimationComponent>(entity, animation["frames_count"].get_or(1),
                                                         animation["frames_per_second"].get_or(1));
                }
                const sol::optional<sol::table> maybeBoxCollider{components["box_collider"]};
                if (maybeBoxCollider) {
                    const sol::table boxCollider{maybeBoxCollider.value()};
                    registry.emplace<BoxColliderComponent>(
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
                const sol::optional<sol::table> maybeHealth{components["health"]};
                if (maybeHealth) {
                    const sol::table health{maybeHealth.value()};
                    registry.emplace<HealthComponent>(entity, health["health_percentage"].get_or(100));
                }
                const sol::optional<sol::table> maybeProjectileEmitter{components["projectile_emitter"]};
                if (maybeProjectileEmitter) {
                    const sol::table projectileEmitter{maybeProjectileEmitter.value()};
                    registry.emplace<ProjectileEmitterComponent>(
                        entity,
                        glm::vec2{projectileEmitter["projectile_velocity"]["x"],
                                  projectileEmitter["projectile_velocity"]["y"]},
                        projectileEmitter["cooldown"].get_or(1.0f) * 1000,
                        projectileEmitter["projectile_duration"].get_or(10.0f) * 1000,
                        projectileEmitter["hit_percentage_damage"].get_or(10),
                        projectileEmitter["is_projectile_friendly"].get_or(false),
                        projectileEmitter["is_auto_shoot"].get_or(true));
                }
                if (components["player"]) {
                    registry.emplace<Player>(entity);
                }
                if (components["enemy"]) {
                    registry.emplace<Enemy>(entity);
                }
            }
        }
    }

    LevelData LevelLoader::loadLevel(const std::filesystem::path& levelFilepath, entt::registry& registry,
                                     AssetManager& assetManager, Renderer& renderer)
    {
        sol::state luaState{};
        luaState.open_libraries(sol::lib::base);
        sol::load_result loadResult{luaState.load_file(levelFilepath)};
        if (!loadResult.valid()) {
            sol::error error{loadResult};
            Logger::error("Error loading lua script: {}", error.what());
            return {};
        }
        loadResult();
        const sol::table level{luaState["Level"]};
        loadAssets(level, assetManager, renderer);
        auto mapData{loadTilemap(level, registry)};
        loadEntities(level, registry);
        return LevelData{mapData};
    }
} // namespace Engine
