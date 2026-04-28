#include "ResourceManager.h"

#include "core/Filesystem.h"
#include "core/Locator.h"

#include <array>
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
            const size_t v{y / (height / 2)};
            for (size_t x{0}; x < width * channels; ++x) {
                const size_t rgbComponent{x % channels};
                const size_t u{x / (width * channels / 2) * channels + rgbComponent};
                fallbackTextureData.at(y * width * channels + x) =
                    canonicalCheckeredData.at(v * 2 * channels + u);
            }
        }
        m_fallbackTexture = std::make_unique<Texture2D>(TextureConfig{});
        m_fallbackTexture->Create(fallbackTextureData.data(), width, height, GL_RGB);
    }

    std::filesystem::path ResourceManager::GetResourcePath(const std::filesystem::path& relativePath) const
    {
        return Filesystem::GetResourcesPath() / relativePath;
    }

    void ResourceManager::Clear() { m_textures.clear(); }

    void ResourceManager::LoadTexture(const std::filesystem::path& relativeFilepath,
                                      const TextureConfig& textureConfig)
    {
        const std::filesystem::path filepath{Filesystem::GetResourcesPath() / relativeFilepath};
        int width{};
        int height{};
        int channels{};
        bool ok{static_cast<bool>(stbi_info(filepath.c_str(), &width, &height, &channels))};
        if (!ok) {
            Locator::GetLogger()->Error("Failed to get info from texture file {}: {}", filepath.c_str(),
                                        stbi_failure_reason());
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
        unsigned char* data{stbi_load(filepath.c_str(), &width, &height, &channels, desiredChannels)};
        if (!data) {
            Locator::GetLogger()->Error("Failed to open texture file {}: {}", filepath.c_str(),
                                        stbi_failure_reason());
            return;
        }
        auto texture{std::make_unique<Texture2D>(textureConfig)};
        texture->Create(data, width, height, imageFormat);
        stbi_image_free(data);
        StringId textureId{relativeFilepath.c_str()};
        m_textures.insert(std::make_pair(textureId, std::move(texture)));
        Locator::GetLogger()->Info("Texture loaded from {} as '{}'", filepath.c_str(), textureId.GetSid());
    }

    const Texture2D& ResourceManager::GetTexture(const StringId& textureId) const
    {
        const auto textureIterator{m_textures.find(textureId)};
        if (textureIterator != m_textures.end()) {
            return *textureIterator->second;
        }
        return *m_fallbackTexture;
    }
} // namespace Engine
