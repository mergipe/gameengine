#include "Renderer.h"
#include "core/Logger.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Engine
{
    Renderer::Renderer(const Window& window)
        : m_renderingContext(SDL_CreateRenderer(window.getWindowPtr(), nullptr))
    {
        if (!m_renderingContext) {
            Logger::critical("Failed to initialize the renderer: {}", SDL_GetError());
            std::abort();
        }
        Logger::info("Renderer initialized");
    }

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(m_renderingContext);
        m_renderingContext = nullptr;
        Logger::info("Renderer destroyed");
    }

    std::unique_ptr<Texture> Renderer::loadTexture(const std::filesystem::path& filepath)
    {
        const char* filepathStr{filepath.c_str()};
        SDL_Texture* texture{IMG_LoadTexture(m_renderingContext, filepathStr)};
        if (!texture) {
            Logger::error("Failed to load texture from {}: {}", filepath.c_str(), SDL_GetError());
        }
        Logger::info("Texture loaded from {}", filepathStr);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        return std::make_unique<Texture>(filepathStr, texture);
    }

    void Renderer::setDrawColor(const Color& color)
    {
        SDL_SetRenderDrawColor(m_renderingContext, color.r, color.g, color.b, color.a);
    }

    void Renderer::drawRectangle(FRect rect, bool fill)
    {
        if (fill) {
            SDL_RenderFillRect(m_renderingContext, &rect);
        } else {
            SDL_RenderRect(m_renderingContext, &rect);
        }
    }

    void Renderer::drawTexture(const Texture& texture, const FRect& sourceRect, const FRect& destinationRect,
                               double rotationAngle)
    {
        SDL_RenderTextureRotated(m_renderingContext, texture.getData(), &sourceRect, &destinationRect,
                                 rotationAngle, nullptr, SDL_FLIP_NONE);
    }

    void Renderer::drawText(const Font& font, std::string_view text, const Color& color, glm::vec2 position)
    {
        SDL_Surface* surface{TTF_RenderText_Blended(font.getData(), text.data(), text.length(), color)};
        SDL_Texture* texture{SDL_CreateTextureFromSurface(m_renderingContext, surface)};
        SDL_DestroySurface(surface);
        float textWidth{};
        float textHeight{};
        SDL_GetTextureSize(texture, &textWidth, &textHeight);
        FRect destinationRect{position.x, position.y, textWidth, textHeight};
        SDL_RenderTexture(m_renderingContext, texture, nullptr, &destinationRect);
        SDL_DestroyTexture(texture);
    }

    void Renderer::clear() { SDL_RenderClear(m_renderingContext); }

    void Renderer::present() { SDL_RenderPresent(m_renderingContext); }
} // namespace Engine
