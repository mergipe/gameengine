#include "AssetManager.h"
#include "core/Logger.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <utility>

namespace Engine
{
    void AssetManager::addTexture(std::string_view assetId, std::unique_ptr<Texture> texture)
    {
        Logger::info("Adding texture from {} to the asset manager as '{}'", texture->getFilepath().c_str(),
                     assetId);
        m_textures.insert(std::make_pair(assetId, std::move(texture)));
    }

    void AssetManager::addFont(std::string_view assetId, const std::filesystem::path& relativePath,
                               float fontSize)
    {
        Logger::info("Adding font from {} to the asset manager as '{}'", relativePath.c_str(), assetId);
        auto font{std::make_unique<Font>(TTF_OpenFont(relativePath.c_str(), fontSize))};
        m_fonts.insert(std::make_pair(assetId, std::move(font)));
    }
} // namespace Engine
