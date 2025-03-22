#ifndef DEVGUI_H
#define DEVGUI_H

#include "core/Window.h"

namespace Engine
{
    struct DevGuiData {
        bool showMetricsOverlay{true};
        bool showMetricsWindow{false};
        bool showDemoWindow{false};
    };

    class DevGui
    {
    public:
        DevGui(const Window& window);
        DevGui(const DevGui&) = delete;
        DevGui(DevGui&&) = delete;
        DevGui& operator=(const DevGui&) = delete;
        DevGui& operator=(DevGui&&) = delete;
        ~DevGui();
        void processEvent(SDL_Event& event);
        bool wantCaptureKeyboard();
        bool wantCaptureMouse();
        void newFrame();
        void render();
        void show();

    private:
        DevGuiData m_data{};
    };
} // namespace Engine

#endif
