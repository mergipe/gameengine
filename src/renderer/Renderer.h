#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"
#include "Texture.h"
#include "core/Window.h"
#include <SDL_render.h>
#include <filesystem>
#include <memory>

namespace Engine
{
    class Renderer
    {
    public:
        explicit Renderer(const Window& window);
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        ~Renderer();
        std::unique_ptr<Texture> loadTexture(const std::filesystem::path& filepath);
        void setDrawColor(const Color& color);
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
