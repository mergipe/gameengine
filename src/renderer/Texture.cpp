#include "Texture.h"

namespace Engine
{
    Texture::~Texture()
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
} // namespace Engine
