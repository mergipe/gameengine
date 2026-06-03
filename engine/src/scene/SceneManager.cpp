#include "SceneManager.h"

#include "Components.h"
#include "core/Locator.h"
#include "core/StringId.h"
#include "core/Yaml.h"
#include "resources/EntityLoader.h"

#include <glm/gtc/type_ptr.inl>
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
                ImGui::MenuItem("Entities", nullptr, &m_debugUIData.showEntityEditor);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (m_debugUIData.showEntityEditor) {
            if (ImGui::Begin("Entity editor", &m_debugUIData.showEntityEditor)) {
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
                if (ImGui::BeginChild("entity_edit_pane")) {
                    if (auto* idComponent{m_sceneContext.registry.try_get<IdComponent>(selectedEntity)}) {
                        ImGui::Text("ID: %s", idComponent->value.GetString().data());
                    }
                    if (auto* tagComponent{m_sceneContext.registry.try_get<TagComponent>(selectedEntity)}) {
                        ImGui::Text("Tag: %s", tagComponent->value.GetString().data());
                    }
                    ImGuiTreeNodeFlags treeNodeFlags{ImGuiTreeNodeFlags_DefaultOpen};
                    if (auto* transformComponent{
                            m_sceneContext.registry.try_get<TransformComponent>(selectedEntity)}) {
                        if (ImGui::CollapsingHeader("Transform", treeNodeFlags)) {
                            ImGui::DragFloat3("Position", glm::value_ptr(transformComponent->position), 0.1f);
                            glm::vec3 rotation{glm::degrees(transformComponent->rotation)};
                            ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f);
                            transformComponent->rotation = glm::radians(rotation);
                            ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent->scale), 0.01f);
                        }
                    }
                    if (auto* spriteComponent{
                            m_sceneContext.registry.try_get<SpriteComponent>(selectedEntity)}) {
                        if (ImGui::CollapsingHeader("Sprite", treeNodeFlags)) {
                            ImGui::Text("Texture ID: %s", spriteComponent->textureId.GetString().data());
                            ImGui::Text("Sprite ID: %s", spriteComponent->spriteId.GetString().data());
                            ImGui::DragFloat2("Pivot point", glm::value_ptr(spriteComponent->pivotPoint),
                                              0.01f, 0.0f, 1.0f);
                            float color[4] = {
                                spriteComponent->color.r / 255.0f, spriteComponent->color.g / 255.0f,
                                spriteComponent->color.b / 255.0f, spriteComponent->color.a / 255.0f};
                            ImGui::ColorEdit4("Color", color, ImGuiColorEditFlags_Uint8);
                            spriteComponent->color.r = static_cast<U8>(color[0] * 255.0f);
                            spriteComponent->color.g = static_cast<U8>(color[1] * 255.0f);
                            spriteComponent->color.b = static_cast<U8>(color[2] * 255.0f);
                            spriteComponent->color.a = static_cast<U8>(color[3] * 255.0f);
                            ImGui::InputInt("Z-index", &spriteComponent->zIndex);
                        }
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }

    void SceneManager::OnViewportResize(int width, int height)
    {
        if (m_currentScene) {
            m_currentScene->OnViewportResize(width, height);
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
