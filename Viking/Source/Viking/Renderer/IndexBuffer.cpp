#include "vipch.h"

#include "IndexBuffer.h"

#include "Viking/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Viking
{
    Ref<IndexBuffer> IndexBuffer::create(void* data, size_t count)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            VI_CORE_ASSERT(false, "RendererAPI is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLIndexBuffer>(data, count);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
