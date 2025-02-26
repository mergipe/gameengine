#include "Texture.h"
#include "core/Logger.h"

namespace Engine
{
    Texture::~Texture()
    {
        SDL_DestroyTexture(m_data);
        m_data = nullptr;
        Logger::info("Texture from {} destroyed", m_filepath.c_str());
    }
} // namespace Engine
