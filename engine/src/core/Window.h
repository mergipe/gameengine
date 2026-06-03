#ifndef WINDOW_H
#define WINDOW_H

#include "ConfigManager.h"

#include <SDL3/SDL.h>

namespace Engine
{
    struct WindowSize {
        int width{};
        int height{};
    };

    class Window
    {
    public:
        void Create(const WindowConfig& config);
        void Destroy();
        [[nodiscard]] SDL_Window* GetWindowHandle() const { return m_windowHandle; }
        [[nodiscard]] SDL_GLContext GetGLContext() const { return m_glContext; }
        [[nodiscard]] WindowSize GetSize() const;
        [[nodiscard]] float GetDisplayScale() const;

    private:
        SDL_Window* m_windowHandle{};
        SDL_GLContext m_glContext{};
    };
} // namespace Engine

#endif
