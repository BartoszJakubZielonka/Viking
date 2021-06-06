#pragma once

namespace Viking
{
    //Currently Viking only supports 32-bit index buffer.
    class IndexBuffer
    {
    public:
        IndexBuffer() = default;
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        [[nodiscard]] virtual uint32_t getCount() const = 0;

        [[nodiscard]] static Ref<IndexBuffer> create(void* data, size_t count);
    };
}