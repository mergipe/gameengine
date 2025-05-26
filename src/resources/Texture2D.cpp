#include "Texture2D.h"

namespace Engine
{
    Texture2D::Texture2D(const TextureConfig& config)
        : m_minFilter(static_cast<GLint>(config.minFilter))
        , m_magFilter{static_cast<GLint>(config.magFilter)}
        , m_wrapS{static_cast<GLint>(config.wrapX)}
        , m_wrapT{static_cast<GLint>(config.wrapY)}
    {
    }

    Texture2D::~Texture2D() { glDeleteTextures(1, &m_id); }

    void Texture2D::create(const unsigned char* data, GLsizei width, GLsizei height, GLint imageFormat)
    {
        m_width = width;
        m_height = height;
        m_internalFormat = imageFormat;
        m_imageFormat = static_cast<GLenum>(imageFormat);
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat,
                     GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }
} // namespace Engine
