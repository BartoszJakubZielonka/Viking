#pragma once

namespace Viking
{
    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t shaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        }

        VI_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string mName;
        ShaderDataType mType;
        uint32_t mSize;
        uint32_t mOffset;
        bool mNormalized;

        BufferElement() = default;
        BufferElement(const ShaderDataType type, std::string name, const bool normalized = false) : mName(std::move(name)), mType(type), mSize(shaderDataTypeSize(type)), mOffset(0), mNormalized(normalized)
        {
        }

        [[nodiscard]] uint32_t getComponentCount() const
        {
            switch (mType)
            {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3 * 3;
            case ShaderDataType::Mat4:    return 4 * 4;
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            }

            VI_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements), mStride(0)
        {
            calculateOffsetsAndStride();
        }

        [[nodiscard]] uint32_t getStride() const;
        [[nodiscard]] const std::vector<BufferElement>& getElements() const;

        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const;
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const;
    private:
        void calculateOffsetsAndStride();

        std::vector<BufferElement> mElements;
        uint32_t mStride;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setData(const void* data, uint32_t size) = 0;

        [[nodiscard]] virtual const BufferLayout& getLayout() const = 0;
        virtual void setLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> create(uint32_t size);
        static Ref<VertexBuffer> create(void* data, size_t size);
    };
}