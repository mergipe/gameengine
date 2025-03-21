#include "Window.h"
#include "Logger.h"

namespace Engine
{
    Window::Window(const WindowConfig& config)
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
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        Logger::info("Window destroyed");
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
        m_window = SDL_CreateWindow(m_config.title.c_str(), m_config.width, m_config.height, flags);
        if (!m_window) {
            Logger::critical("Failed to create a window: {}", SDL_GetError());
            std::abort();
        }
        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            Logger::critical("Failed to create OpenGL context: {}", SDL_GetError());
            std::abort();
        }
        SDL_GL_MakeCurrent(m_window, m_glContext);
        Logger::info("Window initialized");
    }
} // namespace Engine
