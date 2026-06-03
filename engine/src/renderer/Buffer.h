#ifndef BUFFER_H
#define BUFFER_H

#include "core/Types.h"

#include <memory>
#include <vector>

namespace Engine
{
    struct VertexAttribute {
        U32 index{};
        U32 type{};
        int size{};
        bool normalized{};
    };

    class Buffer
    {
    public:
        virtual ~Buffer() = default;
        void Create(U32 size);
        void Create(U32 size, const void* data);
        void Bind();
        void Unbind();
        void UpdateData(U32 offset, U32 size, const void* data);
        [[nodiscard]] U32 GetSize() const;
        void Destroy();
        [[nodiscard]] U32 GetId() const;

    protected:
        explicit Buffer(U32 type);

    private:
        void Create(U32 size, const void* data, U32 usage);

        U32 m_id{};
        U32 m_type{};
        U32 m_size{};
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer();
        void Create(U32 size, bool isInstanced = false);
        void Create(U32 size, const void* data);
        void AddAttribute(const VertexAttribute& attribute);
        [[nodiscard]] int GetStride() const;
        [[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const;
        [[nodiscard]] bool IsInstanced() const;

    private:
        std::vector<VertexAttribute> m_attributes{};
        int m_stride{};
        bool m_isInstanced{false};
    };

    class UniformBuffer : public Buffer
    {
    public:
        UniformBuffer();
        void Create(U32 size, U32 bindingPoint);
        [[nodiscard]] U32 GetBindingPoint() const;

    private:
        U32 m_bindingPoint{};
    };

    class ElementBuffer : public Buffer
    {
    public:
        ElementBuffer();

    private:
        U32 m_size{};
    };

    class VertexArray
    {
    public:
        void Create();
        void Bind();
        void Unbind();
        void AddVertexBuffer(VertexBuffer&& buffer);
        void SetElementBuffer(ElementBuffer&& buffer);
        [[nodiscard]] VertexBuffer* GetVertexBuffer(std::size_t index);
        [[nodiscard]] ElementBuffer* GetElementBuffer();
        void ConfigureVertexAttributes();
        void Destroy();

    private:
        std::vector<VertexBuffer> m_vertexBuffers{};
        std::unique_ptr<ElementBuffer> m_elementBuffer{};
        U32 m_id{};
    };
} // namespace Engine

#endif
