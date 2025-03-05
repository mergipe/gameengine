#ifndef DEVELOPERMODEGUI_H
#define DEVELOPERMODEGUI_H

#include "core/Window.h"
#include "renderer/Renderer.h"

namespace Engine
{
    class DeveloperModeGui
    {
    public:
        DeveloperModeGui(const Window& window, Renderer* renderer);
        DeveloperModeGui(const DeveloperModeGui&) = delete;
        DeveloperModeGui(DeveloperModeGui&&) = delete;
        DeveloperModeGui& operator=(const DeveloperModeGui&) = delete;
        DeveloperModeGui& operator=(DeveloperModeGui&&) = delete;
        ~DeveloperModeGui();
        void processEvent(SDL_Event& event);
        void newFrame();
        void render();
        void showDemoWindow();

    private:
        Renderer* m_renderer;
    };
} // namespace Engine

#endif
