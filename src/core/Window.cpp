#include "Window.h"
#include "Logger.h"

namespace Engine
{
    Window::Window(std::string_view title, int width, int height)
        : m_title{title}, m_width{width}, m_height{height}
    {
        m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width,
                                    m_height, SDL_WINDOW_BORDERLESS);
        if (!m_window) {
            Logger::critical("Failed to create a window: {}", SDL_GetError());
            std::abort();
        }
        Logger::info("Window initialized");
    }

    Window::~Window()
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        Logger::info("Window destroyed");
    }
} // namespace Engine
