#include "Texture2D.h"

#include "core/StringId.h"

#include <optional>

namespace Engine
{
    constexpr std::optional<TextureFiltering> ParseTextureFiltering(const StringId& filteringName)
    {
        using enum TextureFiltering;
        if (filteringName == SID("nearest"))
            return nearest;
        if (filteringName == SID("linear"))
            return linear;
        return {};
    }

    constexpr std::optional<TextureWrapping> ParseTextureWrapping(const StringId& wrappingName)
    {
        using enum TextureWrapping;
        if (wrappingName == SID("repeat"))
            return repeat;
        if (wrappingName == SID("mirroredRepeat"))
            return mirroredRepeat;
        if (wrappingName == SID("clampToEdge"))
            return clampToEdge;
        return {};
    }

    Sprite Sprite::ParseFromYAML(const YAML::Node& rootNode)
    {
        Sprite sprite{};
        sprite.id = StringId::Intern(rootNode["id"].as<std::string>(std::string{}));
        const int x{rootNode["x"].as<int>(0)};
        const int y{rootNode["y"].as<int>(0)};
        sprite.uvTopLeft = glm::vec2{x, y};
        sprite.size = glm::vec2{rootNode["width"].as<float>(0.0f), rootNode["height"].as<float>(0.0f)};
        return sprite;
    }

    TextureConfig TextureConfig::ParseFromYAML(const YAML::Node& rootNode)
    {
        TextureConfig config{};
        if (rootNode["min_filter"]) {
            const StringId minFilterSid{
                StringId::Intern(rootNode["min_filter"].as<std::string>(std::string{}))};
            const std::optional minFilter{ParseTextureFiltering(minFilterSid)};
            if (minFilter) {
                config.minFilter = *minFilter;
            }
        }
        if (rootNode["mag_filter"]) {
            const StringId magFilterSid{
                StringId::Intern(rootNode["mag_filter"].as<std::string>(std::string{}))};
            const std::optional magFilter{ParseTextureFiltering(magFilterSid)};
            if (magFilter) {
                config.magFilter = *magFilter;
            }
        }
        if (rootNode["wrap_x"]) {
            const StringId wrapXSid{StringId::Intern(rootNode["wrap_x"].as<std::string>(std::string{}))};
            const std::optional wrapX{ParseTextureWrapping(wrapXSid)};
            if (wrapX) {
                config.wrapX = *wrapX;
            }
        }
        if (rootNode["wrap_y"]) {
            const StringId wrapYSid{StringId::Intern(rootNode["wrap_y"].as<std::string>(std::string{}))};
            const std::optional wrapY{ParseTextureWrapping(wrapYSid)};
            if (wrapY) {
                config.wrapY = *wrapY;
            }
        }
        if (rootNode["generate_mipmap"]) {
            config.generateMipmap = rootNode["generate_mipmap"].as<bool>();
        }
        if (rootNode["mipmap_filter"]) {
            const StringId mipmapFilterSid{StringId::Intern(rootNode["mipmap_filter"].as<std::string>())};
            const std::optional mipmapFilter{ParseTextureFiltering(mipmapFilterSid)};
            if (mipmapFilter) {
                config.mipmapFilter = *mipmapFilter;
            }
        }
        if (rootNode["sprites"]) {
            const auto spritesNode{rootNode["sprites"]};
            for (auto it{spritesNode.begin()}; it != spritesNode.end(); ++it) {
                auto sprite{Sprite::ParseFromYAML(*it)};
                config.sprites[sprite.id] = sprite;
            }
        }
        return config;
    }

    Texture2D::Texture2D(const TextureConfig& config)
        : m_sprites{config.sprites}
        , m_magFilter{static_cast<GLint>(config.magFilter)}
        , m_wrapS{static_cast<GLint>(config.wrapX)}
        , m_wrapT{static_cast<GLint>(config.wrapY)}
        , m_generateMipmap{config.generateMipmap}
    {
        if (config.generateMipmap) {
            if (config.minFilter == TextureFiltering::linear) {
                if (config.mipmapFilter == TextureFiltering::linear) {
                    m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
                } else {
                    m_minFilter = GL_LINEAR_MIPMAP_NEAREST;
                }
            } else {
                if (config.mipmapFilter == TextureFiltering::linear) {
                    m_minFilter = GL_NEAREST_MIPMAP_LINEAR;
                } else {
                    m_minFilter = GL_NEAREST_MIPMAP_NEAREST;
                }
            }
        } else {
            m_minFilter = static_cast<GLint>(config.minFilter);
        }
    }

    Texture2D::~Texture2D() { glDeleteTextures(1, &m_id); }

    bool Texture2D::operator==(const Texture2D& other) const { return m_id == other.m_id; }

    const Sprite* Texture2D::GetSprite(const StringId& spriteId) const
    {
        if (const auto sprite{m_sprites.find(spriteId)}; sprite != m_sprites.end()) {
            return &sprite->second;
        }
        return {};
    }

    void Texture2D::Create(const unsigned char* data, GLsizei width, GLsizei height, GLint imageFormat)
    {
        m_width = width;
        m_height = height;
        m_internalFormat = imageFormat;
        m_imageFormat = static_cast<GLenum>(imageFormat);
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat,
                     GL_UNSIGNED_BYTE, data);
        if (m_generateMipmap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Bind(U32 unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
} // namespace Engine
