#ifndef WINDOW_H
#define WINDOW_H

#include "ConfigManager.h"

#include <SDL3/SDL.h>

namespace Engine
{
    class Window
    {
    public:
        explicit Window(const WindowConfig& config);
        void Init();
        void Close();
        [[nodiscard]] SDL_Window* GetWindowHandle() const { return m_windowHandle; }
        [[nodiscard]] SDL_GLContext GetGLContext() const { return m_glContext; }
        [[nodiscard]] const WindowConfig& GetConfig() const { return m_config; }
        [[nodiscard]] int GetWidth() const { return m_config.width; }
        [[nodiscard]] int GetHeight() const { return m_config.height; }
        [[nodiscard]] float GetDisplayScale() const { return m_displayScale; }

    private:
        WindowConfig m_config{};
        SDL_Window* m_windowHandle{};
        SDL_GLContext m_glContext{};
        float m_displayScale{1.0f};
    };
} // namespace Engine

#endif
