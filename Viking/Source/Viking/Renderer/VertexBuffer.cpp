#include "vipch.h"

#include "VertexBuffer.h"

#include "Viking/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Viking
{
    uint32_t BufferLayout::getStride() const
    {
        return mStride;
    }

    const std::vector<BufferElement>& BufferLayout::getElements() const
    {
        return mElements;
    }

    std::vector<BufferElement>::iterator BufferLayout::begin()
    {
        return mElements.begin();
    }

    std::vector<BufferElement>::iterator BufferLayout::end()
    {
        return mElements.end();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::begin() const
    {
        return mElements.begin();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::end() const
    {
        return mElements.end();
    }

    void BufferLayout::calculateOffsetsAndStride()
    {
        mStride = 0;
        for (auto& element : mElements)
        {
            element.mOffset = mStride;
            mStride += element.mSize;
        }
    }

    Ref<VertexBuffer> VertexBuffer::create(uint32_t size)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLVertexBuffer>(size);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::create(void* data, size_t size)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            VI_CORE_ASSERT(false, "RendererAPI is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLVertexBuffer>(data, size);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
