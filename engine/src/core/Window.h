#ifndef WINDOW_H
#define WINDOW_H

#include "Config.h"

#include <SDL3/SDL.h>

namespace Engine
{
    class Window
    {
    public:
        explicit Window(const Config::WindowConfig& config);
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window();
        void Init();
        SDL_Window* GetWindowHandle() const { return m_windowHandle; }
        SDL_GLContext GetGLContext() const { return m_glContext; }
        const Config::WindowConfig& GetConfig() const { return m_config; }
        int GetWidth() const { return m_config.width; }
        int GetHeight() const { return m_config.height; }
        float GetDisplayScale() const { return m_displayScale; }

    private:
        Config::WindowConfig m_config{};
        SDL_Window* m_windowHandle{};
        SDL_GLContext m_glContext{};
        float m_displayScale{1.0f};
    };
} // namespace Engine

#endif
