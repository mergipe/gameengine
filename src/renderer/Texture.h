#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>
#include <filesystem>

namespace Engine
{
    class Texture
    {
    public:
        Texture(const std::filesystem::path& filepath, SDL_Texture* texture)
            : m_filepath{filepath}, m_data{texture}
        {
        }
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;
        ~Texture();
        const std::filesystem::path& getFilepath() const { return m_filepath; }
        SDL_Texture* getData() const { return m_data; }

    private:
        const std::filesystem::path m_filepath{};
        SDL_Texture* m_data{};
    };
} // namespace Engine

#endif
