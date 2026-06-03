#include "Buffer.h"

#include <glad/glad.h>

namespace Engine
{
    int GetTypeSize(U32 type)
    {
        switch (type) {
        case GL_BYTE:
            return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        case GL_SHORT:
            return sizeof(GLshort);
        case GL_UNSIGNED_SHORT:
            return sizeof(GLushort);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_DOUBLE:
            return sizeof(GLdouble);
        default:
            return 0;
        }
    }

    void Buffer::Create(U32 size) { Create(size, nullptr, GL_DYNAMIC_DRAW); }

    void Buffer::Create(U32 size, const void* data) { Create(size, data, GL_STATIC_DRAW); }

    void Buffer::Bind() { glBindBuffer(m_type, m_id); }

    void Buffer::Unbind() { glBindBuffer(m_type, 0); }

    void Buffer::UpdateData(U32 offset, U32 size, const void* data)
    {
        Bind();
        glBufferSubData(m_type, offset, size, data);
        Unbind();
    }

    U32 Buffer::GetSize() const { return m_size; }

    void Buffer::Destroy()
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
        m_size = 0;
    }

    U32 Buffer::GetId() const { return m_id; }

    Buffer::Buffer(U32 type)
        : m_type{type}
    {
    }

    void Buffer::Create(U32 size, const void* data, U32 usage)
    {
        m_size = size;
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(m_type, size, data, usage);
        Unbind();
    }

    VertexBuffer::VertexBuffer()
        : Buffer{GL_ARRAY_BUFFER}
    {
    }

    void VertexBuffer::Create(U32 size, bool isInstanced)
    {
        Buffer::Create(size);
        m_isInstanced = isInstanced;
    }

    void VertexBuffer::Create(U32 size, const void* data) { Buffer::Create(size, data); }

    void VertexBuffer::AddAttribute(const VertexAttribute& attribute)
    {
        m_stride += attribute.size * GetTypeSize(attribute.type);
        m_attributes.push_back(attribute);
    }

    int VertexBuffer::GetStride() const { return m_stride; }

    const std::vector<VertexAttribute>& VertexBuffer::GetAttributes() const { return m_attributes; }

    bool VertexBuffer::IsInstanced() const { return m_isInstanced; }

    UniformBuffer::UniformBuffer()
        : Buffer{GL_UNIFORM_BUFFER}
    {
    }

    void UniformBuffer::Create(U32 size, U32 bindingPoint)
    {
        Buffer::Create(size);
        m_bindingPoint = bindingPoint;
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, GetId());
    }

    U32 UniformBuffer::GetBindingPoint() const { return m_bindingPoint; }

    ElementBuffer::ElementBuffer()
        : Buffer{GL_ELEMENT_ARRAY_BUFFER}
    {
    }

    void VertexArray::Create() { glGenVertexArrays(1, &m_id); }

    void VertexArray::Bind() { glBindVertexArray(m_id); }

    void VertexArray::Unbind() { glBindVertexArray(0); }

    void VertexArray::AddVertexBuffer(VertexBuffer&& buffer)
    {
        m_vertexBuffers.emplace_back(std::move(buffer));
    }

    void VertexArray::SetElementBuffer(ElementBuffer&& buffer)
    {
        m_elementBuffer = std::make_unique<ElementBuffer>(std::move(buffer));
        Bind();
        m_elementBuffer->Bind();
        Unbind();
    }

    VertexBuffer* VertexArray::GetVertexBuffer(std::size_t index) { return &m_vertexBuffers[index]; }

    ElementBuffer* VertexArray::GetElementBuffer() { return m_elementBuffer.get(); }

    void VertexArray::ConfigureVertexAttributes()
    {
        Bind();
        for (auto& buffer : m_vertexBuffers) {
            int offset{0};
            buffer.Bind();
            for (const auto& attribute : buffer.GetAttributes()) {
                glEnableVertexAttribArray(attribute.index);
                if (!attribute.normalized &&
                    (attribute.type == GL_BYTE || attribute.type == GL_UNSIGNED_BYTE ||
                     attribute.type == GL_SHORT || attribute.type == GL_UNSIGNED_SHORT ||
                     attribute.type == GL_INT || attribute.type == GL_UNSIGNED_INT)) {
                    glVertexAttribIPointer(attribute.index, attribute.size, attribute.type,
                                           buffer.GetStride(), reinterpret_cast<void*>(offset));
                } else {
                    glVertexAttribPointer(attribute.index, attribute.size, attribute.type,
                                          attribute.normalized, buffer.GetStride(),
                                          reinterpret_cast<const void*>(offset));
                }
                glVertexAttribDivisor(attribute.index, static_cast<int>(buffer.IsInstanced()));
                offset += attribute.size * GetTypeSize(attribute.type);
            }
            buffer.Unbind();
        }
        Unbind();
    }

    void VertexArray::Destroy()
    {
        for (auto& buffer : m_vertexBuffers) {
            buffer.Destroy();
        }
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
} // namespace Engine
