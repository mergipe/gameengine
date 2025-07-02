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
#include <yaml-cpp/yaml.h>

namespace Engine
{
    TextureConfig getTextureConfig(const YAML::Node& textureNode)
    {
        TextureConfig config{};
        if (textureNode["min_filter"]) {
            const std::optional minFilter{
                getTextureFiltering(textureNode["min_filter"].as<std::string>(std::string{}))};
            if (minFilter) {
                config.minFilter = *minFilter;
            }
        }
        if (textureNode["mag_filter"]) {
            const std::optional magFilter{
                getTextureFiltering(textureNode["mag_filter"].as<std::string>(std::string{}))};
            if (magFilter) {
                config.magFilter = *magFilter;
            }
        }
        if (textureNode["wrap_x"]) {
            const std::optional wrapX{
                getTextureWrapping(textureNode["wrap_x"].as<std::string>(std::string{}))};
            if (wrapX) {
                config.wrapX = *wrapX;
            }
        }
        if (textureNode["wrap_y"]) {
            const std::optional wrapY{
                getTextureWrapping(textureNode["wrap_y"].as<std::string>(std::string{}))};
            if (wrapY) {
                config.wrapY = *wrapY;
            }
        }
        return config;
    }

    void loadAssets(const YAML::Node& assetsNode, ResourceManager& resourceManager)
    {
        if (!assetsNode.IsDefined()) {
            return;
        }
        for (YAML::const_iterator it{assetsNode.begin()}; it != assetsNode.end(); ++it) {
            const YAML::Node assetNode{*it};
            const std::string typeStr{assetNode["type"].as<std::string>(std::string{})};
            const auto type{getResourceType(typeStr)};
            if (!type) {
                Logger::warn("Asset type '{}' unknown", typeStr);
                continue;
            }
            const std::string assetId{assetNode["id"].as<std::string>(std::string{})};
            const std::string filepath{assetNode["file"].as<std::string>(std::string{})};
            switch (type.value()) {
            case ResourceType::texture:
                resourceManager.loadTexture(StringId{assetId}, filepath, getTextureConfig(assetNode));
                break;
            default:
                break;
            }
        }
    }

    MapData loadTilemap(const YAML::Node& tilemapNode, entt::registry& registry)
    {
        const auto& windowConfig{Game::instance().getWindowConfig()};
        if (!tilemapNode.IsDefined()) {
            return {static_cast<float>(windowConfig.width), static_cast<float>(windowConfig.height)};
        }
        const std::string mapFilepath{tilemapNode["map_file"].as<std::string>(std::string{})};
        Logger::info("Loading map from {}", mapFilepath);
        const std::string textureId{tilemapNode["texture_id"].as<std::string>(std::string{})};
        const int mapRows{tilemapNode["map_rows"].as<int>(0)};
        const int mapCols{tilemapNode["map_cols"].as<int>(0)};
        const int tilesetCols{tilemapNode["tileset_cols"].as<int>(0)};
        const float tileSize{tilemapNode["tile_size"].as<float>(0.0f)};
        const float scale{tilemapNode["scale"].as<float>(0.0f)};
        std::ifstream tilemapFile{mapFilepath};
        if (!tilemapFile) {
            Logger::error("Error opening {} map file", mapFilepath);
        }
        const std::vector<std::vector<int>> tilemapCsv{IO::parseIntCsvFile(tilemapFile)};
        tilemapFile.close();
        const float leftX{-static_cast<float>(windowConfig.width) / 2};
        const float topY{static_cast<float>(windowConfig.height) / 2};
        for (size_t i{0}; i < tilemapCsv.size(); ++i) {
            const std::vector<int>& values{tilemapCsv[i]};
            for (size_t j{0}; j < values.size(); ++j) {
                auto tile{registry.create()};
                registry.emplace<TagComponent>(tile, StringId{"tile"});
                registry.emplace<TransformComponent>(
                    tile,
                    glm::vec3{leftX + static_cast<float>(j) * tileSize * scale,
                              topY - static_cast<float>(i) * tileSize * scale, 0.0f},
                    glm::vec3{scale, scale, 0.0f});
                const int tileId{values[j]};
                registry.emplace<SpriteComponent>(
                    tile, StringId{textureId},
                    Rect{glm::vec2{tileSize * static_cast<float>(tileId % tilesetCols),
                                   tileSize * static_cast<float>(tileId) / static_cast<float>(tilesetCols)},
                         tileSize, tileSize, glm::vec2{0.0f, 1.0f}},
                    glm::vec3{1.0f}, 0);
            }
        }
        return MapData{static_cast<float>(mapCols) * tileSize * scale,
                       static_cast<float>(mapRows) * tileSize * scale};
    }

    void loadEntities(const YAML::Node& entitiesNode, entt::registry& registry,
                      ScriptingSystem& scriptingSystem)
    {
        if (!entitiesNode.IsDefined()) {
            return;
        }
        for (YAML::const_iterator it{entitiesNode.begin()}; it != entitiesNode.end(); ++it) {
            const YAML::Node entityNode{*it};
            const YAML::Node componentsNode{entityNode["components"]};
            if (componentsNode.IsDefined()) {
                auto entity{registry.create()};
                const std::string idStr{componentsNode["id"].as<std::string>(std::string{})};
                if (!idStr.empty()) {
                    registry.emplace<IdComponent>(entity, StringId{idStr});
                } else {
                    Logger::warn("Ignoring entity without id");
                    continue;
                }
                const std::string tag{componentsNode["tag"].as<std::string>(std::string{})};
                if (!tag.empty()) {
                    registry.emplace<TagComponent>(entity, StringId{tag});
                }
                const YAML::Node transformNode{componentsNode["transform"]};
                if (transformNode.IsDefined()) {
                    registry.emplace<TransformComponent>(
                        entity,
                        glm::vec3{transformNode["position"]["x"].as<float>(0.0f),
                                  transformNode["position"]["y"].as<float>(0.0f),
                                  transformNode["position"]["z"].as<float>(0.0f)},
                        glm::vec3{transformNode["scale"]["x"].as<float>(1.0f),
                                  transformNode["scale"]["y"].as<float>(1.0f),
                                  transformNode["scale"]["z"].as<float>(1.0f)},
                        glm::vec3{glm::radians(transformNode["rotation"]["x"].as<float>(0.0f)),
                                  glm::radians(transformNode["rotation"]["y"].as<float>(0.0f)),
                                  glm::radians(transformNode["rotation"]["z"].as<float>(0.0f))});
                }
                const YAML::Node rigidBody2DNode{componentsNode["rigid_body"]};
                if (rigidBody2DNode.IsDefined()) {
                    registry.emplace<RigidBody2DComponent>(
                        entity, glm::vec3{rigidBody2DNode["velocity"]["x"].as<float>(0.0f),
                                          rigidBody2DNode["velocity"]["y"].as<float>(0.0f),
                                          rigidBody2DNode["velocity"]["z"].as<float>(0.0f)});
                }
                const YAML::Node spriteNode{componentsNode["sprite"]};
                if (spriteNode.IsDefined()) {
                    const std::string textureId{spriteNode["texture_id"].as<std::string>(std::string{})};
                    registry.emplace<SpriteComponent>(entity, StringId{textureId},
                                                      Rect{glm::vec2{spriteNode["texture_x"].as<float>(0.0f),
                                                                     spriteNode["texture_y"].as<float>(0.0f)},
                                                           spriteNode["width"].as<float>(0.0f),
                                                           spriteNode["height"].as<float>(0.0f),
                                                           glm::vec2{0.0f, 1.0f}},
                                                      glm::vec3{spriteNode["color"]["r"].as<float>(1.0f),
                                                                spriteNode["color"]["g"].as<float>(1.0f),
                                                                spriteNode["color"]["b"].as<float>(1.0f)},
                                                      spriteNode["z_index"].as<int>(0));
                }
                const YAML::Node spriteAnimationNode{componentsNode["sprite_animation"]};
                if (spriteAnimationNode.IsDefined()) {
                    registry.emplace<SpriteAnimationComponent>(
                        entity, spriteAnimationNode["frames_count"].as<int>(1),
                        spriteAnimationNode["frames_per_second"].as<int>(1),
                        spriteAnimationNode["should_loop"].as<bool>(true));
                }
                const YAML::Node boxCollider2DNode{componentsNode["box_collider"]};
                if (boxCollider2DNode.IsDefined()) {
                    registry.emplace<BoxCollider2DComponent>(
                        entity, boxCollider2DNode["width"].as<float>(0.0f),
                        boxCollider2DNode["height"].as<float>(0.0f),
                        glm::vec2{boxCollider2DNode["offset"]["x"].as<float>(0.0f),
                                  boxCollider2DNode["offset"]["y"].as<float>(0.0f)});
                }
                const YAML::Node playerInputNode{componentsNode["player_input"]};
                if (playerInputNode.IsDefined()) {
                    registry.emplace<PlayerInputComponent>(entity);
                }
                const YAML::Node scriptsNode{componentsNode["scripts"]};
                if (scriptsNode.IsDefined()) {
                    std::vector<Script> scripts{};
                    for (YAML::const_iterator it{scriptsNode.begin()}; it != scriptsNode.end(); ++it) {
                        const YAML::Node scriptNode{*it};
                        const std::string filepath{scriptNode["filepath"].as<std::string>(std::string{})};
                        const std::string className{scriptNode["class_name"].as<std::string>(std::string{})};
                        if (!filepath.empty() && !className.empty()) {
                            std::optional maybeScript{
                                scriptingSystem.loadScript(entity, filepath, className)};
                            if (maybeScript) {
                                scripts.push_back(maybeScript.value());
                            }
                        }
                    }
                    registry.emplace<ScriptComponent>(entity, scripts);
                }
                const YAML::Node cameraNode{componentsNode["camera"]};
                if (cameraNode.IsDefined()) {
                    const std::string projectionTypeStr{
                        cameraNode["projection"].as<std::string>(std::string{"orthographic"})};
                    const std::optional projectionType{getProjectionType(projectionTypeStr)};
                    const float zNear{cameraNode["z_near"].as<float>(-1.0f)};
                    const float zFar{cameraNode["z_far"].as<float>(100.0f)};
                    const auto& windowConfig{Game::instance().getWindowConfig()};
                    const float viewportWidth{static_cast<float>(windowConfig.width)};
                    const float viewportHeight{static_cast<float>(windowConfig.height)};
                    std::unique_ptr<Camera> cameraPtr{};
                    if (projectionType == ProjectionType::orthographic) {
                        cameraPtr =
                            std::make_unique<OrthographicCamera>(zNear, zFar, viewportWidth, viewportHeight);
                    } else if (projectionType == ProjectionType::perspective) {
                        const float fovY{cameraNode["fov_y"].as<float>(45.0f)};
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

    SceneData SceneLoader::load(const std::filesystem::path& sceneFilepath, entt::registry& registry,
                                ResourceManager& resourceManager, ScriptingSystem& scriptingSystem)
    {
        Logger::info("Loading scene from {}", sceneFilepath.c_str());
        const YAML::Node rootNode{YAML::LoadFile(sceneFilepath)};
        loadAssets(rootNode["assets"], resourceManager);
        SceneData sceneData{};
        sceneData.mapData = loadTilemap(rootNode["tilemap"], registry);
        loadEntities(rootNode["entities"], registry, scriptingSystem);
        return sceneData;
    }
} // namespace Engine
