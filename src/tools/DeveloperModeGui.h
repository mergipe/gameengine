#ifndef DEVELOPERMODEGUI_H
#define DEVELOPERMODEGUI_H

#include "core/Window.h"

namespace Engine
{
    class DeveloperModeGui
    {
    public:
        DeveloperModeGui(const Window& window);
        DeveloperModeGui(const DeveloperModeGui&) = delete;
        DeveloperModeGui(DeveloperModeGui&&) = delete;
        DeveloperModeGui& operator=(const DeveloperModeGui&) = delete;
        DeveloperModeGui& operator=(DeveloperModeGui&&) = delete;
        ~DeveloperModeGui();
        void processEvent(SDL_Event& event);
        void show();
    };
} // namespace Engine

#endif
