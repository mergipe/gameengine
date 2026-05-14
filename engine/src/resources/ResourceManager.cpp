#include "ResourceManager.h"

#include "EntityLoader.h"
#include "core/FileSystem.h"
#include "core/Locator.h"

#include <array>
#include <stb_image/stb_image.h>
#include <utility>

namespace Engine
{
    std::filesystem::path ResourceManager::GetResourcePath(const std::filesystem::path& relativePath)
    {
        return s_resourcesPath / relativePath;
    }

    void ResourceManager::Init()
    {
        constexpr int width{32};
        constexpr int height{width};
        constexpr int channels{3};
        constexpr std::array<unsigned char, 4 * channels> canonicalCheckerboardData{0,   0, 0,   255, 0, 255,
                                                                                    255, 0, 255, 0,   0, 0};
        std::array<unsigned char, width * height * channels> fallbackTextureData{};
        for (size_t y{0}; y < height; ++y) {
            const size_t v{y / (height / 2)};
            for (size_t x{0}; x < width * channels; ++x) {
                const size_t rgbComponent{x % channels};
                const size_t u{x / (width * channels / 2) * channels + rgbComponent};
                fallbackTextureData.at(y * width * channels + x) =
                    canonicalCheckerboardData.at(v * 2 * channels + u);
            }
        }
        m_fallbackTexture = std::make_unique<Texture2D>(TextureConfig{});
        m_fallbackTexture->Create(fallbackTextureData.data(), width, height, GL_RGB);
        Locator::GetLogger()->Info("Resource manager initialized");
    }

    void ResourceManager::ShutDown() { Locator::GetLogger()->Info("Resource manager shut down"); }

    void ResourceManager::Clear() { m_textures.clear(); }

    void ResourceManager::LoadResource(const std::filesystem::path& relativeFilePath)
    {
        const std::filesystem::path absoluteFilePath{s_resourcesPath / relativeFilePath};
        if (!FileSystem::IsFile(absoluteFilePath)) {
            Locator::GetLogger()->Error("Resource {} not found", absoluteFilePath.c_str());
            return;
        }
        const std::filesystem::path metadataFilePath{absoluteFilePath.string() + s_metadataFileExtension};
        if (!FileSystem::IsFile(metadataFilePath)) {
            Locator::GetLogger()->Error("Resource metadata {} not found", metadataFilePath.c_str());
            return;
        }
        const YAML::Node metadataNode{YAML::LoadFile(metadataFilePath)};
        if (!metadataNode["resource_type"]) {
            Locator::GetLogger()->Error("Missing resource_type field on metadata file {}",
                                        metadataFilePath.c_str());
            return;
        }
        const StringId resourceTypeSid{StringId::Intern(metadataNode["resource_type"].as<std::string>())};
        const auto resourceType{ParseResourceType(resourceTypeSid)};
        if (!resourceType) {
            Locator::GetLogger()->Error("Unknown resource_type: {} on metadata file {}",
                                        resourceTypeSid.GetString(), metadataFilePath.c_str());
            return;
        }
        const StringId resourceId{StringId::Intern(relativeFilePath.c_str())};
        switch (*resourceType) {
        case ResourceType::texture:
            LoadTexture(resourceId, absoluteFilePath, metadataNode);
            break;
        case ResourceType::font:
            LoadFont(resourceId, absoluteFilePath, metadataNode);
            break;
        case ResourceType::entity_template:
            LoadTemplate(resourceId, absoluteFilePath);
            break;
        }
    }

    const Texture2D& ResourceManager::GetTexture(const StringId& id) const
    {
        if (const auto it{m_textures.find(id)}; it != m_textures.end()) {
            return *it->second;
        }
        return *m_fallbackTexture;
    }

    std::optional<entt::handle> ResourceManager::GetTemplate(const StringId& id) const
    {
        if (const auto it{m_templates.find(id)}; it != m_templates.end()) {
            return it->second;
        }
        return {};
    }

    void ResourceManager::LoadTexture(const StringId& id, const std::filesystem::path& filePath,
                                      const YAML::Node& metadataNode)
    {
        int width{};
        int height{};
        int channels{};
        bool ok{static_cast<bool>(stbi_info(filePath.c_str(), &width, &height, &channels))};
        if (!ok) {
            Locator::GetLogger()->Error("Failed to get info from texture file {}: {}", filePath.c_str(),
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
        unsigned char* data{stbi_load(filePath.c_str(), &width, &height, &channels, desiredChannels)};
        if (!data) {
            Locator::GetLogger()->Error("Failed to open texture file {}: {}", filePath.c_str(),
                                        stbi_failure_reason());
            return;
        }
        const auto textureConfig{TextureConfig::ParseFromYAML(metadataNode)};
        auto texture{std::make_unique<Texture2D>(textureConfig)};
        texture->Create(data, width, height, imageFormat);
        stbi_image_free(data);
        m_textures.insert(std::make_pair(id, std::move(texture)));
        Locator::GetLogger()->Info("Texture {} loaded with id {}", filePath.c_str(), id.GetSid());
    }

    void ResourceManager::LoadFont([[maybe_unused]] const StringId& id,
                                   [[maybe_unused]] const std::filesystem::path& filePath,
                                   [[maybe_unused]] const YAML::Node& metadataNode)
    {
    }

    void ResourceManager::LoadTemplate(const StringId& id, const std::filesystem::path& filePath)
    {
        const auto entityNode{YAML::LoadFile(filePath)};
        auto entity{EntityLoader::Load(m_templateRegistry, entityNode)};
        if (m_templateRegistry.valid(entity)) {
            m_templates.emplace(id, entt::handle{m_templateRegistry, entity});
        } else {
            Locator::GetLogger()->Error("Loaded entity {} is not valid!", id.GetString());
        }
    }
} // namespace Engine
