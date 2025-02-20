#include "Renderer.h"
#include "core/Logger.h"
#include <SDL_image.h>

namespace Engine
{
    Renderer::Renderer(const Window& window)
        : m_renderer(SDL_CreateRenderer(window.getWindowPtr(), -1, SDL_RENDERER_ACCELERATED))
    {
        if (!m_renderer) {
            Logger::critical("Failed to initialize the renderer: {}", SDL_GetError());
            std::abort();
        }
        Logger::info("Renderer initialized");
    }

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
        Logger::info("Renderer destroyed");
    }

    std::unique_ptr<Texture> Renderer::loadTexture(const std::filesystem::path& filepath)
    {
        const char* filepathStr{filepath.c_str()};
        SDL_Texture* texture{IMG_LoadTexture(m_renderer, filepathStr)};
        if (!texture) {
            Logger::error("Failed to load texture from {}: {}", filepath.c_str(), IMG_GetError());
        }
        Logger::info("Texture loaded from {}", filepathStr);
        return std::make_unique<Texture>(filepathStr, texture);
    }

    void Renderer::setDrawColor(const Color& color)
    {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    }

    void Renderer::drawRectangle(float x, float y, float width, float height)
    {
        const SDL_FRect rect{x, y, width, height};
        SDL_RenderDrawRectF(m_renderer, &rect);
    }

    void Renderer::drawTexture(const Texture& texture, const SDL_Rect& sourceRect,
                               const SDL_FRect& destinationRect, double rotationAngle)
    {
        SDL_RenderCopyExF(m_renderer, texture.getTexturePtr(), &sourceRect, &destinationRect, rotationAngle,
                          nullptr, SDL_FLIP_NONE);
    }

    void Renderer::clear() { SDL_RenderClear(m_renderer); }

    void Renderer::present() { SDL_RenderPresent(m_renderer); }
} // namespace Engine
