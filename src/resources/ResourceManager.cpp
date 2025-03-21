#include "ResourceManager.h"
#include "core/Logger.h"
#include <stb_image/stb_image.h>
#include <utility>

namespace Engine
{
    std::filesystem::path ResourceManager::getResourcePath(const std::filesystem::path& relativePath) const
    {
        return m_resourcesPath / relativePath;
    }

    void ResourceManager::clear() { m_textures.clear(); }

    void ResourceManager::loadTexture(std::string_view textureId, const std::filesystem::path& relativePath,
                                      const TextureConfig& textureConfig)
    {
        std::filesystem::path path{m_resourcesPath / relativePath};
        int width{};
        int height{};
        int channels{};
        bool ok{static_cast<bool>(stbi_info(path.c_str(), &width, &height, &channels))};
        if (!ok) {
            Logger::error("Failed to get info from texture file {}: {}", path.c_str(), stbi_failure_reason());
            return;
        }
        int desiredChannels{};
        int imageFormat{};
        if (channels <= 3) {
            desiredChannels = STBI_rgb;
            imageFormat = GL_RGB;
        } else {
            desiredChannels = STBI_rgb_alpha;
            imageFormat = GL_RGBA;
        }
        unsigned char* data{stbi_load(path.c_str(), &width, &height, &channels, desiredChannels)};
        if (!data) {
            Logger::error("Failed to open texture file {}: {}", path.c_str(), stbi_failure_reason());
            return;
        }
        auto texture{std::make_unique<Texture2D>(textureConfig)};
        texture->create(data, width, height, imageFormat);
        stbi_image_free(data);
        m_textures.insert(std::make_pair(textureId, std::move(texture)));
        Logger::info("Texture loaded from {}  as '{}'", path.c_str(), textureId);
    }

    const Texture2D& ResourceManager::getTexture(std::string_view textureId) const
    {
        return *(m_textures.at(textureId.data()));
    };
} // namespace Engine
