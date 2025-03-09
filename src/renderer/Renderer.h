#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"
#include "Font.h"
#include "Rect.h"
#include "Texture.h"
#include "core/Window.h"
#include <SDL3/SDL.h>
#include <filesystem>
#include <glm/glm.hpp>
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
        SDL_Renderer* getRenderingContext() const { return m_renderingContext; }
        std::unique_ptr<Texture> loadTexture(const std::filesystem::path& filepath);
        void setDrawColor(const Color& color);
        void drawRectangle(FRect rect, bool fill);
        void drawTexture(const Texture& texture, const FRect& sourceRect, const FRect& destinationRect,
                         float rotationAngle);
        void drawText(const Font& font, std::string_view text, const Color& color, glm::vec2 position);
        void clear();
        void present();

    private:
        SDL_Renderer* m_renderingContext{};
    };
} // namespace Engine

#endif
