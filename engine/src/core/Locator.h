#ifndef LOCATOR_H
#define LOCATOR_H

#include "core/Logger.h"
#include "events/EventBus.h"
#include "input/InputManager.h"
#include "physics/PhysicsEngine2D.h"
#include "renderer/DebugRenderer.h"
#include "renderer/RenderManager.h"
#include "resources/ResourceManager.h"
#include "scene/Systems.h"

namespace Engine
{
    class Locator
    {
    public:
        Locator() = delete;
        static Logger* GetLogger() { return s_logger; }
        static EventBus* GetEventBus() { return s_eventBus; }
        static ResourceManager* GetResourceManager() { return s_resourceManager; }
        static RenderManager* GetRenderManager() { return s_renderManager; }
        static DebugRenderer* GetDebugRenderer() { return s_debugRenderer; }
        static InputManager* GetInputManager() { return s_inputManager; }
        static PhysicsEngine2D* GetPhysicsEngine2D() { return s_physicsEngine2D; }
        static ScriptingSystem* GetScriptingSystem() { return s_scriptingSystem; }
        static void Provide(Logger* logger) { s_logger = logger; }
        static void Provide(EventBus* eventBus) { s_eventBus = eventBus; }
        static void Provide(ResourceManager* resourceManager) { s_resourceManager = resourceManager; }
        static void Provide(RenderManager* renderManager) { s_renderManager = renderManager; }
        static void Provide(DebugRenderer* debugRenderer) { s_debugRenderer = debugRenderer; }
        static void Provide(InputManager* inputManager) { s_inputManager = inputManager; }
        static void Provide(PhysicsEngine2D* physicsEngine2D) { s_physicsEngine2D = physicsEngine2D; }
        static void Provide(ScriptingSystem* scriptingSystem) { s_scriptingSystem = scriptingSystem; }

    private:
        static inline Logger* s_logger{};
        static inline EventBus* s_eventBus{};
        static inline ResourceManager* s_resourceManager{};
        static inline RenderManager* s_renderManager{};
        static inline DebugRenderer* s_debugRenderer{};
        static inline InputManager* s_inputManager{};
        static inline PhysicsEngine2D* s_physicsEngine2D{};
        static inline ScriptingSystem* s_scriptingSystem{};
    };
} // namespace Engine

#endif
