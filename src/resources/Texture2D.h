#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <optional>
#include <string_view>

namespace Engine
{
    enum class TextureFiltering { nearest = GL_NEAREST, linear = GL_LINEAR };
    enum class TextureWrapping {
        repeat = GL_REPEAT,
        mirroredRepeat = GL_MIRRORED_REPEAT,
        clampToEdge = GL_CLAMP_TO_EDGE
    };

    constexpr std::optional<TextureFiltering> parseTextureFiltering(std::string_view filteringStr)
    {
        using enum TextureFiltering;
        if (filteringStr == "nearest")
            return nearest;
        if (filteringStr == "linear")
            return linear;
        return {};
    }

    constexpr std::optional<TextureWrapping> parseTextureWrapping(std::string_view wrappingStr)
    {
        using enum TextureWrapping;
        if (wrappingStr == "repeat")
            return repeat;
        if (wrappingStr == "mirroredRepeat")
            return mirroredRepeat;
        if (wrappingStr == "clampToEdge")
            return clampToEdge;
        return {};
    }

    struct TextureConfig {
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
        GLsizei getWidth() const { return m_width; }
        GLsizei getHeight() const { return m_height; }
        void create(const unsigned char* data, GLsizei width, GLsizei height, GLint imageFormat);
        void bind() const;

    private:
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
