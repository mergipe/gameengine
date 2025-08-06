#include "Window.h"
#include "Locator.h"
#include <cstdlib>

namespace Engine
{
    Window::Window(const Config::WindowConfig& config)
        : m_config{config}
    {
        if (m_config.width <= 0 || m_config.height <= 0) {
            m_config.width = 800;
            m_config.height = 600;
        }
    }

    Window::~Window()
    {
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
        SDL_DestroyWindow(m_windowHandle);
        m_windowHandle = nullptr;
        Locator::getLogger()->info("Window destroyed");
    }

    const SDL_DisplayMode* getLargerDisplayMode()
    {
        int displayCount{};
        const SDL_DisplayID* displays{SDL_GetDisplays(&displayCount)};
        int maxHeight{};
        const SDL_DisplayMode* largerDisplayMode{};
        for (int i{0}; i < displayCount; ++i) {
            const SDL_DisplayMode* displayMode{SDL_GetDesktopDisplayMode(displays[i])};
            if (displayMode->h > maxHeight) {
                maxHeight = displayMode->h;
                largerDisplayMode = displayMode;
            }
        }
        return largerDisplayMode;
    }

    void Window::init()
    {
        SDL_WindowFlags flags{SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY};
        if (m_config.isFullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;
        if (m_config.isResizable)
            flags |= SDL_WINDOW_RESIZABLE;
        if (m_config.isBorderless)
            flags |= SDL_WINDOW_BORDERLESS;
        if (m_config.isMaximized)
            flags |= SDL_WINDOW_MAXIMIZED;
        const SDL_DisplayMode* displayMode{getLargerDisplayMode()};
        if (m_config.width > displayMode->w || m_config.height > displayMode->h) {
            m_config.width = displayMode->w;
            m_config.height = displayMode->h;
        }
        m_windowHandle = SDL_CreateWindow(m_config.title.c_str(), m_config.width, m_config.height, flags);
        if (!m_windowHandle) {
            Locator::getLogger()->critical("Failed to create a window: {}", SDL_GetError());
            std::abort();
        }
        m_glContext = SDL_GL_CreateContext(m_windowHandle);
        if (!m_glContext) {
            Locator::getLogger()->critical("Failed to create OpenGL context: {}", SDL_GetError());
            std::abort();
        }
        SDL_GL_MakeCurrent(m_windowHandle, m_glContext);
        m_displayScale = SDL_GetWindowDisplayScale(m_windowHandle);
        Locator::getLogger()->info("Window initialized");
    }
} // namespace Engine
