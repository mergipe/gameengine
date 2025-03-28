#include "Scene.h"
#include "Game.h"
#include "SceneLoader.h"
#include "Systems.h"

namespace Engine
{
    Scene::Scene(Renderer2D* renderer, ResourceManager* resourceManager, int windowWidth, int windowHeight)
        : m_renderer{renderer}
        , m_resourceManager{resourceManager}
        , m_registry{std::make_unique<entt::registry>()}
        , m_movementSystem{std::make_unique<MovementSystem>(m_registry.get())}
        , m_spriteRenderingSystem{std::make_unique<SpriteRenderingSystem>(m_registry.get())}
        , m_animationSystem{std::make_unique<SpriteAnimationSystem>(m_registry.get())}
        , m_collisionSystem{std::make_unique<CollisionSystem>(m_registry.get())}
        , m_playerInputSystem{std::make_unique<PlayerInputSystem>(m_registry.get())}
        , m_scriptSystem{std::make_unique<ScriptSystem>(m_registry.get())}
    {
        if (Game::instance().hasDevMode()) {
            m_boxColliderRenderingSystem = std::make_unique<BoxColliderRenderingSystem>(m_registry.get());
        }
        SceneLoader sceneLoader{};
        m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
        m_scriptSystem->createScriptBindings(m_lua);
        std::filesystem::path scriptsFolder{m_resourceManager->getResourcePath("scenes")};
        m_sceneData = sceneLoader.load(m_lua, scriptsFolder / "scene1.lua", *m_registry, *resourceManager);
        m_sceneData.camera = {glm::vec2{0.0f, 0.0f}, static_cast<float>(windowWidth),
                              static_cast<float>(windowHeight)};
    }

    Scene::~Scene() { m_resourceManager->clear(); }

    void Scene::update(float timeStep)
    {
        EventBus& eventBus{Game::instance().getEventBus()};
        eventBus.reset();
        m_playerInputSystem->subscribeToEvents(eventBus);
        m_movementSystem->update(timeStep);
        m_collisionSystem->update();
        m_animationSystem->update();
        m_scriptSystem->update(timeStep);
    }

    void Scene::render(float frameExtrapolationTimeStep)
    {
        m_renderer->clear();
        m_spriteRenderingSystem->update(*m_renderer, *m_resourceManager, m_sceneData.camera,
                                        frameExtrapolationTimeStep);
        if (Game::instance().isDevModeEnabled()) {
            m_boxColliderRenderingSystem->update(*m_renderer, m_sceneData.camera, frameExtrapolationTimeStep);
        }
    }
} // namespace Engine
