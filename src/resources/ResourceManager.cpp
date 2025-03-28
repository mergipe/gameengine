#include "ResourceManager.h"
#include "core/Filesystem.h"
#include "core/Logger.h"
#include <stb_image/stb_image.h>
#include <utility>

namespace Engine
{
    ResourceManager::ResourceManager()
    {
        constexpr int width{32};
        constexpr int height{width};
        constexpr int channels{3};
        constexpr std::array<unsigned char, 4 * channels> canonicalCheckeredData{0,   0, 0,   255, 0, 255,
                                                                                 255, 0, 255, 0,   0, 0};
        std::array<unsigned char, width * height * channels> fallbackTextureData{};
        for (size_t y{0}; y < height; ++y) {
            size_t v{y / (height / 2)};
            for (size_t x{0}; x < width * channels; ++x) {
                size_t rgbComponent{x % channels};
                size_t u{(x / (width * channels / 2)) * channels + rgbComponent};
                fallbackTextureData.at(y * width * channels + x) =
                    canonicalCheckeredData.at(v * 2 * channels + u);
            }
        }
        m_fallbackTexture = std::make_unique<Texture2D>(TextureConfig{});
        m_fallbackTexture->create(fallbackTextureData.data(), width, height, GL_RGB);
    }

    std::filesystem::path ResourceManager::getResourcePath(const std::filesystem::path& relativePath) const
    {
        return Filesystem::getResourcesPath() / relativePath;
    }

    void ResourceManager::clear() { m_textures.clear(); }

    void ResourceManager::loadTexture(std::string_view textureId, const std::filesystem::path& relativePath,
                                      const TextureConfig& textureConfig)
    {
        std::filesystem::path path{Filesystem::getResourcesPath() / relativePath};
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
        const auto textureIterator{m_textures.find(textureId.data())};
        if (textureIterator != m_textures.end()) {
            return *textureIterator->second;
        }
        return *m_fallbackTexture;
    };
} // namespace Engine
