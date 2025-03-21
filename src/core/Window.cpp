#include "Window.h"
#include "Logger.h"

namespace Engine
{
    Window::Window(std::string_view title, int width, int height)
        : m_title{title}, m_width{width}, m_height{height}
    {
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
        SDL_WindowFlags flags{SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIGH_PIXEL_DENSITY};
        m_window = SDL_CreateWindow(m_title.c_str(), m_width, m_height, flags);
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
