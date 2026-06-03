#include "Window.h"

#include "Locator.h"

#include <cstdlib>

namespace Engine
{
    void Window::Create(const WindowConfig& config)
    {
        SDL_WindowFlags flags{SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY};
        if (config.isFullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;
        if (config.isResizable)
            flags |= SDL_WINDOW_RESIZABLE;
        if (config.isBorderless)
            flags |= SDL_WINDOW_BORDERLESS;
        if (config.isMaximized)
            flags |= SDL_WINDOW_MAXIMIZED;
        int width{config.width};
        int height{config.height};
        if (width <= 0 || height <= 0) {
            const SDL_DisplayMode* currentDisplayMode{SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay())};
            width = currentDisplayMode->w;
            height = currentDisplayMode->h;
        }
        m_windowHandle = SDL_CreateWindow(config.title.c_str(), width, height, flags);
        if (!m_windowHandle) {
            Locator::GetLogger()->Critical("Failed to create a window: {}", SDL_GetError());
            std::abort();
        }
        m_glContext = SDL_GL_CreateContext(m_windowHandle);
        if (!m_glContext) {
            Locator::GetLogger()->Critical("Failed to create OpenGL context: {}", SDL_GetError());
            std::abort();
        }
        SDL_GL_MakeCurrent(m_windowHandle, m_glContext);
        Locator::GetLogger()->Info("Window initialized");
    }

    void Window::Destroy()
    {
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
        SDL_DestroyWindow(m_windowHandle);
        m_windowHandle = nullptr;
        Locator::GetLogger()->Info("Window closed");
    }

    WindowSize Window::GetSize() const
    {
        WindowSize size{};
        SDL_GetWindowSizeInPixels(m_windowHandle, &size.width, &size.height);
        return size;
    }

    float Window::GetDisplayScale() const { return SDL_GetWindowDisplayScale(m_windowHandle); }
} // namespace Engine
