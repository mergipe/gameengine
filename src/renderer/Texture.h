#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL_render.h>
#include <filesystem>

namespace Engine
{
    class Texture
    {
    public:
        Texture(const std::filesystem::path& filepath, SDL_Texture* texture)
            : m_filepath{filepath}, m_texture{texture}
        {
        }
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;
        ~Texture();
        const std::filesystem::path& getFilepath() const { return m_filepath; }
        SDL_Texture* getTexturePtr() const { return m_texture; }

    private:
        const std::filesystem::path m_filepath{};
        SDL_Texture* m_texture{};
    };
} // namespace Engine

#endif
