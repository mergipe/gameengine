#include "SceneManager.h"

#include "Components.h"
#include "core/Locator.h"
#include "core/StringId.h"
#include "core/Yaml.h"
#include "resources/EntityLoader.h"

#include <entt/meta/factory.hpp>
#include <imgui.h>
#include <string>

namespace Engine
{
    void LoadResources(const YAML::Node& resourcesNode)
    {
        if (!resourcesNode) {
            return;
        }
        auto* resourceManager{Locator::GetResourceManager()};
        for (auto it{resourcesNode.begin()}; it != resourcesNode.end(); ++it) {
            const YAML::Node resourceNode{*it};
            resourceManager->LoadResource(resourceNode.as<std::string>());
        }
    }

    void SceneManager::Init()
    {
        m_sceneContext.scriptingSystem = std::make_unique<ScriptingSystem>(&m_sceneContext.registry);
        Locator::Provide(m_sceneContext.scriptingSystem.get());
        // TODO: initializes registry storages for template copying; find a better way
        m_sceneContext.registry.storage<IdComponent>();
        m_sceneContext.registry.storage<TagComponent>();
        m_sceneContext.registry.storage<TransformComponent>();
        m_sceneContext.registry.storage<SpriteComponent>();
        m_sceneContext.registry.storage<SpriteAnimationComponent>();
        m_sceneContext.registry.storage<RigidBody2DComponent>();
        m_sceneContext.registry.storage<BoxCollider2DComponent>();
        m_sceneContext.registry.storage<CircleCollider2DComponent>();
        m_sceneContext.registry.storage<PlayerInputComponent>();
        m_sceneContext.registry.storage<ScriptComponent>();
        m_sceneContext.registry.storage<CameraComponent>();
        entt::meta_factory<IdComponent>{}.type("Id").data<&IdComponent::value>("value");
        entt::meta_factory<TagComponent>{}.type("Tag").data<&TagComponent::value>("value");
        entt::meta_factory<TransformComponent>{}
            .type("Transform")
            .data<&TransformComponent::position>("position")
            .data<&TransformComponent::scale>("scale")
            .data<&TransformComponent::rotation>("rotation");
        entt::meta_factory<SpriteComponent>{}
            .type("Sprite")
            .data<&SpriteComponent::textureId>("textureId")
            .data<&SpriteComponent::spriteId>("spriteId")
            .data<&SpriteComponent::color>("color")
            .data<&SpriteComponent::zIndex>("zIndex");
        entt::meta_factory<SpriteAnimationComponent>{}.type("SpriteAnimation");
        entt::meta_factory<RigidBody2DComponent>{}
            .type("RigidBody2D")
            .data<&RigidBody2DComponent::bodyData>("bodyData");
        entt::meta_factory<BoxCollider2DComponent>{}
            .type("BoxCollider2D")
            .data<&BoxCollider2DComponent::shapeData>("shapeData")
            .data<&BoxCollider2DComponent::width>("width")
            .data<&BoxCollider2DComponent::height>("height");
        entt::meta_factory<CircleCollider2DComponent>{}
            .type("CircleCollider2D")
            .data<&CircleCollider2DComponent::shapeData>("shapeData")
            .data<&CircleCollider2DComponent::radius>("radius");
        entt::meta_factory<PlayerInputComponent>{}
            .type("PlayerInput")
            .data<&PlayerInputComponent::callbackDefs>("callbackMapping")
            .data<&PlayerInputComponent::defaultInputScope>("defaultInputScope");
        entt::meta_factory<ScriptRuntimeComponent>{}
            .type("Script")
            .data<&ScriptRuntimeComponent::scriptInstances>("scriptInstances");
        // entt::meta_factory<CameraComponent>{}.type("Camera").data<&CameraComponent::camera>("camera");
        Locator::GetLogger()->Info("Scene manager initialized");
    }

    void SceneManager::ShutDown()
    {
        m_sceneContext.registry.clear();
        m_currentScene.reset();
        Locator::GetLogger()->Info("Scene manager shut down");
    }

    void SceneManager::LoadScene(const StringId& sceneId)
    {
        m_sceneContext.registry.clear(); // TODO: improve entity management between scenes
        if (FileSystem::IsFile(sceneId.GetString())) {
            Locator::GetLogger()->Error("Scene {} not found!", sceneId.GetString());
            return;
        }
        const YAML::Node rootNode{YAML::LoadFile(ResourceManager::GetResourcePath(sceneId.GetString()))};
        LoadResources(rootNode["resources"]);
        LoadEntities(rootNode["entities"]);
        m_currentScene = std::make_unique<Scene>(&m_sceneContext);
    }

    void SceneManager::RenderDevGui()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Scene")) {
                ImGui::MenuItem("Entities", nullptr, &m_uiData.showEntityEditor);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (m_uiData.showEntityEditor) {
            if (ImGui::Begin("Entity editor", &m_uiData.showEntityEditor)) {
                static entt::entity selectedEntity{};
                if (ImGui::BeginChild("entity_list_pane", ImVec2{150, 0},
                                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX)) {
                    for (const auto view{m_sceneContext.registry.view<IdComponent>()};
                         const auto entity : view) {
                        const auto id{view.get<IdComponent>(entity)};
                        if (ImGui::Selectable(id.value.GetString().data(), selectedEntity == entity)) {
                            selectedEntity = entity;
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::SameLine();
                if (ImGui::BeginChild("entity_view_pane")) {
                    for (auto [storageId, storage] : m_sceneContext.registry.storage()) {
                        if (storage.contains(selectedEntity)) {
                            const auto type{entt::resolve(storage.info())};
                            if (!type)
                                continue;
                            if (ImGui::CollapsingHeader(type.name(), ImGuiTreeNodeFlags_DefaultOpen)) {
                                for (auto [dataId, data] : type.data()) {
                                    ImGui::TextUnformatted(data.name());
                                }
                            }
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::End();
            }
        }
    }

    void SceneManager::LoadEntities(const YAML::Node& entitiesNode)
    {
        if (!entitiesNode) {
            return;
        }
        for (auto entityIt{entitiesNode.begin()}; entityIt != entitiesNode.end(); ++entityIt) {
            const auto& entityNode{*entityIt};
            EntityLoader::Load(m_sceneContext.registry, entityNode);
        }
    }
} // namespace Engine
