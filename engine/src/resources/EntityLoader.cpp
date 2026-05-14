#include "EntityLoader.h"

#include "core/Locator.h"
#include "scene/Components.h"

namespace Engine
{
    void ParseShape2DDef(Shape2DData& shapeDef, const YAML::Node& node)
    {
        if (node["density"]) {
            shapeDef.density = node["density"].as<float>();
        }
        if (node["is_trigger"]) {
            shapeDef.isTrigger = node["is_trigger"].as<bool>();
        }
        if (const auto& materialNode{node["material"]}) {
            Material2DData materialData{};
            if (materialNode["friction"]) {
                materialData.friction = materialNode["friction"].as<float>();
            }
            if (materialNode["restitution"]) {
                materialData.restitution = materialNode["restitution"].as<float>();
            }
            if (materialNode["rolling_resistance"]) {
                materialData.rollingResistance = materialNode["rolling_resistance"].as<float>();
            }
            if (materialNode["tangent_speed"]) {
                materialData.tangentSpeed = materialNode["tangent_speed"].as<float>();
            }
            shapeDef.materialData = materialData;
        }
    }

    void LoadTransform(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& transform{entityHandle.get_or_emplace<TransformComponent>()};
        if (node["position"]) {
            transform.position = node["position"].as<glm::vec3>();
        }
        if (node["scale"]) {
            transform.scale = node["scale"].as<glm::vec3>();
        }
        if (node["rotation"]) {
            const glm::vec3 rotationInDegrees{node["rotation"].as<glm::vec3>()};
            transform.rotation =
                glm::vec3{glm::radians(rotationInDegrees.x), glm::radians(rotationInDegrees.y),
                          glm::radians(rotationInDegrees.z)};
        }
    }

    void LoadTag(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& tag{entityHandle.get_or_emplace<TagComponent>()};
        tag.value = StringId::Intern(node.as<std::string>(std::string{}));
    }

    void LoadRigidBody2D(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& rigidBody{entityHandle.get_or_emplace<RigidBody2DComponent>()};
        if (node["type"]) {
            const StringId typeSid{StringId::Intern(node["type"].as<std::string>())};
            if (const auto bodyType{ParseBodyType2D(typeSid)}) {
                rigidBody.bodyData.type = *bodyType;
            }
        }
        if (node["gravity_scale"]) {
            rigidBody.bodyData.gravityScale = node["gravity_scale"].as<float>();
        }
        if (node["angular_damping"]) {
            rigidBody.bodyData.angularDamping = node["angular_damping"].as<float>();
        }
        if (node["linear_damping"]) {
            rigidBody.bodyData.linearDamping = node["linear_damping"].as<float>();
        }
    }

    void LoadBoxCollider2D(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& boxCollider{entityHandle.get_or_emplace<BoxCollider2DComponent>()};
        ParseShape2DDef(boxCollider.shapeData, node);
        if (node["width"]) {
            boxCollider.width = node["width"].as<float>();
        }
        if (node["height"]) {
            boxCollider.height = node["height"].as<float>();
        }
    }

    void LoadCircleCollider2D(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& circleCollider{entityHandle.get_or_emplace<CircleCollider2DComponent>()};
        ParseShape2DDef(circleCollider.shapeData, node);
        if (node["radius"]) {
            circleCollider.radius = node["radius"].as<float>();
        }
    }

    void LoadSprite(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& spriteComponent{entityHandle.get_or_emplace<SpriteComponent>()};
        if (node["texture_id"]) {
            spriteComponent.textureId = StringId::Intern(node["texture_id"].as<std::string>());
        }
        if (node["sprite_id"]) {
            spriteComponent.spriteId = StringId::Intern(node["sprite_id"].as<std::string>());
        }
        if (node["color"]) {
            spriteComponent.color = node["color"].as<glm::vec3>();
        }
        if (node["z_index"]) {
            spriteComponent.zIndex = node["z_index"].as<int>();
        }
    }

    void LoadSpriteAnimation(const entt::handle& entityHandle, const YAML::Node& node)
    {
        entityHandle.get_or_emplace<SpriteAnimationComponent>();
        if (node[""]) {
        }
    }

    void LoadScripts(const entt::handle& entityHandle, const YAML::Node& node)
    {
        for (auto it{node.begin()}; it != node.end(); ++it) {
            const YAML::Node scriptNode{*it};
            ScriptDef scriptDef{};
            if (scriptNode["file_path"]) {
                scriptDef.filePath = scriptNode["file_path"].as<std::string>();
            }
            if (scriptNode["class_name"]) {
                scriptDef.className = scriptNode["class_name"].as<std::string>();
            }
            if (!scriptDef.filePath.empty() && !scriptDef.className.empty()) {
                auto& scriptComponent{entityHandle.get_or_emplace<ScriptComponent>()};
                scriptComponent.scriptDefs.push_back(scriptDef);
            }
        }
    }

    void LoadPlayerInput(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& playerInput{entityHandle.get_or_emplace<PlayerInputComponent>()};
        if (node["default_scope"]) {
            playerInput.defaultInputScope = StringId::Intern(node["default_scope"].as<std::string>());
        }
        if (node["commands"]) {
            const auto commandsNode{node["commands"]};
            for (auto it{commandsNode.begin()}; it != commandsNode.end(); ++it) {
                const auto commandNode{*it};
                if (!commandNode["name"]) {
                    continue;
                }
                const StringId commandName{StringId::Intern(commandNode["name"].as<std::string>())};
                auto& callbackDef{playerInput.callbackDefs[commandName]};
                if (commandNode["script_id"]) {
                    callbackDef.scriptId = StringId::Intern(commandNode["script_id"].as<std::string>());
                }
                if (commandNode["callback"]) {
                    callbackDef.callbackName = commandNode["callback"].as<std::string>();
                }
            }
        }
    }

    void LoadCamera(const entt::handle& entityHandle, const YAML::Node& node)
    {
        auto& cameraComponent{entityHandle.get_or_emplace<CameraComponent>()};
        if (node["projection"]) {
            const StringId projectionTypeSid{StringId::Intern(node["projection"].as<std::string>())};
            if (const auto projectionType{ParseProjectionType(projectionTypeSid)}) {
                cameraComponent.camera.SetProjectionType(*projectionType);
            }
        }
        if (node["viewport"]) {
            const auto viewport{node["viewport"].as<glm::vec2>()};
            cameraComponent.camera.SetViewport(viewport.x, viewport.y);
        }
        if (node["z_near"]) {
            cameraComponent.camera.SetZNear(node["z_near"].as<float>());
        }
        if (node["z_far"]) {
            cameraComponent.camera.SetZFar(node["z_far"].as<float>());
        }
        if (node["fov_y"]) {
            cameraComponent.camera.SetFovY(glm::radians(node["fov_y"].as<float>()));
        }
    }

    void CopyEntity(const entt::handle& source, const entt::handle& destination)
    {
        for (auto [id, sourceStorage] : source.storage()) {
            if (sourceStorage.contains(source.entity())) {
                if (auto* destinationStorage{destination.registry()->storage(id)}) {
                    destinationStorage->push(destination.entity(), sourceStorage.value(source.entity()));
                }
            }
        }
    }

    entt::entity EntityLoader::Load(entt::registry& registry, const YAML::Node& entityNode)
    {
        const auto& componentsNode{entityNode["components"]};
        if (!componentsNode) {
            return entt::null;
        }
        const auto entity{registry.create()};
        const auto entityHandle{entt::handle{registry, entity}};
        if (componentsNode["id"]) {
            entityHandle.emplace<IdComponent>(StringId::Intern(componentsNode["id"].as<std::string>()));
        }
        std::optional<StringId> parentTemplateId{};
        if (componentsNode["template"]) {
            parentTemplateId = StringId::Intern(componentsNode["template"].as<std::string>());
            if (const auto entityTemplate{Locator::GetResourceManager()->GetTemplate(*parentTemplateId)}) {
                CopyEntity(*entityTemplate, entityHandle);
            } else {
                Locator::GetLogger()->Error("Template {} not found", parentTemplateId->GetString());
            }
        }
        if (componentsNode["transform"]) {
            LoadTransform(entityHandle, componentsNode["transform"]);
        } else if (!entityHandle.any_of<TransformComponent>()) {
            entityHandle.emplace<TransformComponent>();
        }
        if (componentsNode["tag"]) {
            LoadTag(entityHandle, componentsNode["tag"]);
        }
        if (componentsNode["rigid_body_2d"]) {
            LoadRigidBody2D(entityHandle, componentsNode["rigid_body_2d"]);
        }
        if (componentsNode["box_collider_2d"]) {
            LoadBoxCollider2D(entityHandle, componentsNode["box_collider_2d"]);
        }
        if (componentsNode["circle_collider_2d"]) {
            LoadCircleCollider2D(entityHandle, componentsNode["circle_collider_2d"]);
        }
        if (componentsNode["sprite"]) {
            LoadSprite(entityHandle, componentsNode["sprite"]);
        }
        if (componentsNode["sprite_animation"]) {
            LoadSpriteAnimation(entityHandle, componentsNode["sprite_animation"]);
        }
        if (componentsNode["scripts"]) {
            LoadScripts(entityHandle, componentsNode["scripts"]);
        }
        if (componentsNode["player_input"]) {
            LoadPlayerInput(entityHandle, componentsNode["player_input"]);
        }
        if (componentsNode["camera"]) {
            LoadCamera(entityHandle, componentsNode["camera"]);
        }
        return entity;
    }
} // namespace Engine
