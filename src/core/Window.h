#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <string>

namespace Engine
{
    struct WindowConfig {
        std::string title{"Title"};
        int width{800};
        int height{600};
        bool isFullscreen{false};
        bool isResizable{true};
        bool isBorderless{false};
    };

    class Window
    {
    public:
        Window(const WindowConfig& config);
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window();
        void init();
        SDL_Window* getWindowPtr() const { return m_window; }
        SDL_GLContext getGLContext() const { return m_glContext; }
        const WindowConfig& getConfig() const { return m_config; }
        int getWidth() const { return m_config.width; }
        int getHeight() const { return m_config.height; }

    private:
        WindowConfig m_config{};
        SDL_Window* m_window{};
        SDL_GLContext m_glContext{};
    };
} // namespace Engine

#endif
