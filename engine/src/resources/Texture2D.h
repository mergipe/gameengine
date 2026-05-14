#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "core/StringId.h"
#include "core/Yaml.h"
#include "renderer/Shapes.h"

#include <glad/glad.h>
#include <unordered_map>

namespace Engine
{
    enum class TextureFiltering { nearest = GL_NEAREST, linear = GL_LINEAR };
    enum class TextureWrapping {
        repeat = GL_REPEAT,
        mirroredRepeat = GL_MIRRORED_REPEAT,
        clampToEdge = GL_CLAMP_TO_EDGE
    };

    struct Sprite {
        static Sprite ParseFromYAML(const YAML::Node& rootNode);
        StringId id{};
        Rect textureArea{};
    };

    struct TextureConfig {
        static TextureConfig ParseFromYAML(const YAML::Node& rootNode);
        std::unordered_map<StringId, Sprite> sprites{};
        TextureFiltering minFilter{TextureFiltering::linear};
        TextureFiltering magFilter{TextureFiltering::linear};
        TextureWrapping wrapX{TextureWrapping::repeat};
        TextureWrapping wrapY{TextureWrapping::repeat};
        TextureFiltering mipmapFilter{TextureFiltering::linear};
        bool generateMipmap{false};
    };

    class Texture2D
    {
    public:
        explicit Texture2D(const TextureConfig& config);
        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&&) = delete;
        ~Texture2D();
        GLsizei GetWidth() const { return m_width; }
        GLsizei GetHeight() const { return m_height; }
        std::optional<Sprite> GetSprite(const StringId& spriteId) const;
        void Create(const unsigned char* data, GLsizei width, GLsizei height, GLint imageFormat);
        void Bind() const;

    private:
        std::unordered_map<StringId, Sprite> m_sprites{};
        GLuint m_id{};
        GLsizei m_width{};
        GLsizei m_height{};
        GLint m_internalFormat{};
        GLenum m_imageFormat{};
        GLint m_minFilter{};
        GLint m_magFilter{};
        GLint m_wrapS{};
        GLint m_wrapT{};
        bool m_generateMipmap{};
    };
} // namespace Engine

#endif
