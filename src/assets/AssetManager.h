#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "renderer/Font.h"
#include "renderer/Texture.h"
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    class AssetManager final
    {
    public:
        explicit AssetManager(const std::filesystem::path& assetsPath)
            : m_assetsPath{assetsPath}
        {
        }
        AssetManager(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager& operator=(AssetManager&&) = delete;
        ~AssetManager() = default;
        void clearAssets() { m_textures.clear(); }
        void addTexture(std::string_view assetId, std::unique_ptr<Texture> texture);
        const Texture& getTexture(const std::string& assetId) const { return *(m_textures.at(assetId)); };
        void addFont(std::string_view assetId, const std::filesystem::path& relativePath, int fontSize);
        const Font& getFont(const std::string& assetId) const { return *(m_fonts.at(assetId)); }
        const std::filesystem::path& getAssetsPath() const { return m_assetsPath; };
        std::filesystem::path getAssetPath(const std::filesystem::path& relativeFilepath) const
        {
            return m_assetsPath / relativeFilepath;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures{};
        std::unordered_map<std::string, std::unique_ptr<Font>> m_fonts{};
        const std::filesystem::path m_assetsPath{};
    };
} // namespace Engine

#endif
