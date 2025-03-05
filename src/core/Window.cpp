#include "Window.h"
#include "Logger.h"

namespace Engine
{
    Window::Window(std::string_view title, int width, int height)
        : m_title{title}
        , m_window{SDL_CreateWindow(m_title.c_str(), width, height,
                                    SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIGH_PIXEL_DENSITY)}
        , m_width{width}
        , m_height{height}
    {
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
