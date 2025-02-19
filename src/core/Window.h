#ifndef WINDOW_H
#define WINDOW_H

#include <SDL_video.h>
#include <string>
#include <string_view>

namespace Engine
{
    class Window
    {
    public:
        Window(std::string_view title, int width, int height);
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window();
        SDL_Window* getWindowPtr() const { return m_window; }

    private:
        std::string m_title{};
        SDL_Window* m_window{};
        int m_width{};
        int m_height{};
    };
} // namespace Engine

#endif
