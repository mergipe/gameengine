#include "SceneLoader.h"
#include "Components.h"
#include "core/IO.h"
#include "core/Logger.h"
#include "core/StringId.h"
#include "game/Game.h"
#include "renderer/Camera.h"
#include "resources/ResourceManager.h"
#include "resources/Texture2D.h"
#include <fstream>
#include <glm/glm.hpp>
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
            getTextureWrapping(texture["wrap_x"].get_or(std::string{}))};
        if (wrapX) {
            config.wrapX = *wrapX;
        }
        const std::optional<TextureWrapping> wrapY{
            getTextureWrapping(texture["wrap_y"].get_or(std::string{}))};
        if (wrapY) {
            config.wrapY = *wrapY;
        }
        return config;
    }

    void loadAssets(const sol::table& scene, ResourceManager& resourceManager)
    {
        const sol::optional<sol::table> maybeAssets{scene["assets"]};
        if (!maybeAssets) {
            return;
        }
        for (const auto& entry : maybeAssets.value()) {
            const sol::table asset{entry.second};
            const std::string typeStr{asset["type"].get_or(std::string{})};
            const auto type{getResourceType(typeStr)};
            if (!type) {
                Logger::warn("Asset type '{}' unknown", typeStr);
                continue;
            }
            const std::string assetId{asset["id"].get_or(std::string{})};
            const std::string filepath{asset["file"].get_or(std::string{})};
            switch (type.value()) {
            case ResourceType::texture:
                resourceManager.loadTexture(internString(assetId), filepath, getTextureConfig(asset));
                break;
            default:
                break;
            }
        }
    }

    MapData loadMap(const sol::table& scene, entt::registry& registry)
    {
        const sol::optional<sol::table> maybeTilemap{scene["tilemap"]};
        const auto& windowConfig{Game::instance().getWindowConfig()};
        if (!maybeTilemap) {
            return {static_cast<float>(windowConfig.width), static_cast<float>(windowConfig.height)};
        }
        const sol::table tilemap{maybeTilemap.value()};
        const std::string mapFilepath{tilemap["map_file"].get_or(std::string{})};
        Logger::info("Loading map from {}", mapFilepath);
        const std::string textureId{tilemap["texture_id"].get_or(std::string{})};
        const int mapRows{tilemap["map_rows"].get_or(0)};
        const int mapCols{tilemap["map_cols"].get_or(0)};
        const int tilesetCols{tilemap["tileset_cols"].get_or(0)};
        const float tileSize{tilemap["tile_size"].get_or(0.0f)};
        const float scale{tilemap["scale"].get_or(0.0f)};
        std::ifstream tilemapFile{mapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} map file", mapFilepath);
        }
        const std::vector<std::vector<int>> tilemapCsv{IO::parseIntCsvFile(tilemapFile)};
        tilemapFile.close();
        const float leftX{-static_cast<float>(windowConfig.width) / 2};
        const float topY{static_cast<float>(windowConfig.height) / 2};
        for (size_t i{0}; i < tilemapCsv.size(); ++i) {
            const std::vector<int> values{tilemapCsv[i]};
            for (size_t j{0}; j < values.size(); ++j) {
                auto tile{registry.create()};
                registry.emplace<TagComponent>(tile, internString("tile"));
                registry.emplace<TransformComponent>(
                    tile,
                    glm::vec3{leftX + static_cast<float>(j) * tileSize * scale,
                              topY - static_cast<float>(i) * tileSize * scale, 0.0f},
                    glm::vec3{scale, scale, 0.0f});
                const int tileId{values[j]};
                registry.emplace<SpriteComponent>(
                    tile, internString(textureId),
                    Rect{glm::vec2{tileSize * static_cast<float>(tileId % tilesetCols),
                                   tileSize * static_cast<float>(tileId / tilesetCols)},
                         tileSize, tileSize, glm::vec2{0.0f, 1.0f}},
                    glm::vec3{1.0f}, 0);
            }
        }
        return MapData{static_cast<float>(mapCols) * tileSize * scale,
                       static_cast<float>(mapRows) * tileSize * scale};
    }

    void loadEntities(const sol::table& scene, entt::registry& registry)
    {
        const sol::optional<sol::table> maybeEntities{scene["entities"]};
        if (!maybeEntities) {
            return;
        }
        for (const auto& entry : maybeEntities.value()) {
            const sol::table entityTable{entry.second};
            auto entity{registry.create()};
            const sol::optional<sol::table> maybeComponents{entityTable["components"]};
            if (maybeComponents) {
                const sol::table components{maybeComponents.value()};
                const std::string id{components["id"].get_or(std::string{})};
                if (!id.empty()) {
                    registry.emplace<IdComponent>(entity, internString(id));
                }
                const std::string tag{components["tag"].get_or(std::string{})};
                if (!tag.empty()) {
                    registry.emplace<TagComponent>(entity, internString(tag));
                }
                const sol::optional<sol::table> maybeTransform{components["transform"]};
                if (maybeTransform) {
                    const sol::table transform{maybeTransform.value()};
                    registry.emplace<TransformComponent>(
                        entity,
                        glm::vec3{transform["position"]["x"].get_or(0.0f),
                                  transform["position"]["y"].get_or(0.0f),
                                  transform["position"]["z"].get_or(0.0f)},
                        glm::vec3{transform["scale"]["x"].get_or(1.0f), transform["scale"]["y"].get_or(1.0f),
                                  transform["scale"]["z"].get_or(1.0f)},
                        glm::vec3{glm::radians(transform["rotation"]["x"].get_or(0.0f)),
                                  glm::radians(transform["rotation"]["y"].get_or(0.0f)),
                                  glm::radians(transform["rotation"]["z"].get_or(0.0f))});
                }
                const sol::optional<sol::table> maybeRigidBody2D{components["rigid_body"]};
                if (maybeRigidBody2D) {
                    const sol::table rigidBody2D{maybeRigidBody2D.value()};
                    registry.emplace<RigidBody2DComponent>(
                        entity, glm::vec3{rigidBody2D["velocity"]["x"].get_or(0.0f),
                                          rigidBody2D["velocity"]["y"].get_or(0.0f),
                                          rigidBody2D["velocity"]["z"].get_or(0.0f)});
                }
                const sol::optional<sol::table> maybeSprite{components["sprite"]};
                if (maybeSprite) {
                    const sol::table sprite{maybeSprite.value()};
                    const std::string textureId{sprite["texture_id"].get_or(std::string{})};
                    registry.emplace<SpriteComponent>(
                        entity, internString(textureId),
                        Rect{glm::vec2{sprite["texture_x"].get_or(0.0f), sprite["texture_y"].get_or(0.0f)},
                             sprite["width"].get_or(0.0f), sprite["height"].get_or(0.0f),
                             glm::vec2{0.0f, 1.0f}},
                        glm::vec3{sprite["color"]["r"].get_or(1.0f), sprite["color"]["g"].get_or(1.0f),
                                  sprite["color"]["b"].get_or(1.0f)},
                        sprite["z_index"].get_or(0));
                }
                const sol::optional<sol::table> maybeSpriteAnimation{components["sprite_animation"]};
                if (maybeSpriteAnimation) {
                    const sol::table spriteAnimation{maybeSpriteAnimation.value()};
                    registry.emplace<SpriteAnimationComponent>(entity,
                                                               spriteAnimation["frames_count"].get_or(1),
                                                               spriteAnimation["frames_per_second"].get_or(1),
                                                               spriteAnimation["should_loop"].get_or(true));
                }
                const sol::optional<sol::table> maybeBoxCollider2D{components["box_collider"]};
                if (maybeBoxCollider2D) {
                    const sol::table boxCollider2D{maybeBoxCollider2D.value()};
                    registry.emplace<BoxCollider2DComponent>(
                        entity, boxCollider2D["width"].get_or(0.0f), boxCollider2D["height"].get_or(0.0f),
                        glm::vec2{boxCollider2D["offset"]["x"].get_or(0.0f),
                                  boxCollider2D["offset"]["y"].get_or(0.0f)});
                }
                const sol::optional<sol::table> maybePlayerInput{components["player_input"]};
                if (maybePlayerInput) {
                    registry.emplace<PlayerInputComponent>(entity);
                }
                const sol::optional<sol::function> maybeScriptOnUpdate{components["script"]["on_update"]};
                if (maybeScriptOnUpdate) {
                    registry.emplace<ScriptComponent>(entity, maybeScriptOnUpdate.value());
                }
                const sol::optional<sol::table> maybeCamera{components["camera"]};
                if (maybeCamera) {
                    const sol::table camera{maybeCamera.value()};
                    const std::string projectionTypeStr{
                        camera["projection"].get_or(std::string{"orthographic"})};
                    const std::optional<ProjectionType> projectionType{getProjectionType(projectionTypeStr)};
                    const float zNear{camera["z_near"].get_or(-1.0f)};
                    const float zFar{camera["z_far"].get_or(100.0f)};
                    const auto& windowConfig{Game::instance().getWindowConfig()};
                    const float viewportWidth{static_cast<float>(windowConfig.width)};
                    const float viewportHeight{static_cast<float>(windowConfig.height)};
                    std::unique_ptr<Camera> cameraPtr{};
                    if (projectionType == ProjectionType::orthographic) {
                        cameraPtr =
                            std::make_unique<OrthographicCamera>(zNear, zFar, viewportWidth, viewportHeight);
                    } else if (projectionType == ProjectionType::perspective) {
                        const float fovY{camera["fov_y"].get_or(45.0f)};
                        cameraPtr = std::make_unique<PerspectiveCamera>(zNear, zFar, viewportWidth,
                                                                        viewportHeight, glm::radians(fovY));
                    }
                    if (cameraPtr) {
                        registry.emplace<CameraComponent>(entity, std::move(cameraPtr));
                    }
                }
            }
        }
    }

    SceneData SceneLoader::load(sol::state& lua, const std::filesystem::path& sceneFilepath,
                                entt::registry& registry, ResourceManager& resourceManager)
    {
        Logger::info("Loading scene from {}", sceneFilepath.c_str());
        const auto result{lua.script_file(sceneFilepath, sol::script_pass_on_error)};
        if (!result.valid()) {
            const sol::error error{result};
            const sol::call_status status{result.status()};
            Logger::error("Error loading scene file {}: {} error\n\t{}", sceneFilepath.c_str(),
                          sol::to_string(status), error.what());
            return {};
        }
        const sol::optional<sol::table> maybeScene{lua["Scene"]};
        if (!maybeScene) {
            Logger::error("Error parsing scene: Scene table not found!", sceneFilepath.c_str());
            return {};
        }
        const sol::table scene{maybeScene.value()};
        loadAssets(scene, resourceManager);
        SceneData sceneData{};
        sceneData.mapData = loadMap(scene, registry);
        loadEntities(scene, registry);
        return sceneData;
    }
} // namespace Engine
