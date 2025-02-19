#ifndef RENDERER_H
#define RENDERER_H

#include "core/Window.h"
#include "resources/Texture.h"
#include <SDL_render.h>
#include <cstdint>

namespace Engine
{
    class Renderer
    {
    public:
        Renderer(const Window& window);
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        ~Renderer();
        SDL_Renderer* getRendererPtr() { return m_renderer; }
        void setDrawColor(std::uint8_t red, std::uint8_t gree, std::uint8_t blue, std::uint8_t alpha);
        void drawRectangle(float x, float y, float width, float height);
        void drawTexture(const Texture& texture, const SDL_Rect& sourceRect, const SDL_FRect& destinationRect,
                         double rotationAngle);
        void clear();
        void present();

    private:
        SDL_Renderer* m_renderer{};
    };
} // namespace Engine

#endif
