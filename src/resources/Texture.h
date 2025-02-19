#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL_render.h>

namespace Engine
{
    class Texture
    {
    public:
        Texture(SDL_Texture* texture)
            : m_texture{texture}
        {
        }
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;
        ~Texture();
        SDL_Texture* getTexturePtr() const { return m_texture; }

    private:
        SDL_Texture* m_texture{};
    };
} // namespace Engine

#endif
