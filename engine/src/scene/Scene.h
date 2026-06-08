#pragma once

#include "events/EventBus.h"
#include "events/Events.h"

namespace Engine
{
    class Scene final
    {
    public:
        explicit Scene(entt::registry* registry);
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;
        ~Scene();

        void Start();
        void Update(float timeStep);
        void Render(float frameExtrapolationTimeStep);
        void OnViewportResize(int width, int height);

    private:
        void StartPlayerInput();
        void StartPhysics2D();
        void StartScripts();
        void OnInputCommand(const InputEvent& event);
        void UpdatePlayerInput(EventBus* eventBus);
        void UpdatePhysics2D(float timeStep);
        void UpdateScripts(float timeStep);

        entt::registry* m_registry{};
    };
} // namespace Engine
