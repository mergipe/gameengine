#include "ResourceManager.h"
#include "core/Logger.h"
#include <utility>

namespace Engine
{
    void ResourceManager::addTexture(std::string_view resourceId, std::unique_ptr<Texture> texture)
    {
        Logger::info("Adding texture from {} to the resource manager as '{}'", texture->getFilepath().c_str(),
                     resourceId);
        m_textures.insert(std::make_pair(resourceId, std::move(texture)));
    }
} // namespace Engine
