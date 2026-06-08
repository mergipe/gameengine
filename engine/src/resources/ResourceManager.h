#pragma once

#include "Texture2D.h"
#include "core/FileSystem.h"
#include "core/StringId.h"
#include "core/Yaml.h"

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    enum class ResourceType { texture, font, entity_template };

    constexpr std::optional<ResourceType> ParseResourceType(const StringId& resourceTypeName)
    {
        using enum ResourceType;
        if (resourceTypeName == SID("texture"))
            return texture;
        if (resourceTypeName == SID("font"))
            return font;
        if (resourceTypeName == SID("template"))
            return entity_template;
        return {};
    }

    class ResourceManager final
    {
    public:
        static std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath);
        void Init();
        void ShutDown();
        void Clear();
        void LoadResource(const std::filesystem::path& relativeFilePath);
        const Texture2D& GetTexture(const StringId& id) const;
        std::optional<entt::handle> GetTemplate(const StringId& id) const;

    private:
        static inline const std::filesystem::path s_resourcesPath{FileSystem::GetAbsolutePath("resources")};
        static inline const auto s_metadataFileExtension{".metadata"};
        void LoadTexture(const StringId& id, const std::filesystem::path& filePath,
                         const YAML::Node& metadataNode);
        void LoadFont(const StringId& id, const std::filesystem::path& filePath,
                      const YAML::Node& metadataNode);
        void LoadTemplate(const StringId& id, const std::filesystem::path& filePath);
        entt::registry m_templateRegistry{};
        std::unordered_map<StringId, entt::handle> m_templates{};
        std::unordered_map<StringId, std::unique_ptr<Texture2D>> m_textures{};
        std::unique_ptr<Texture2D> m_fallbackTexture{};
    };
} // namespace Engine
