//
// Created by batzi on 16.05.2021.
//

#include "vipch.h"
#include "Viking/Renderer/Buffer.h"

#include "Viking/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Viking {
    Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(size);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(vertices, size);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t count) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLIndexBuffer>(indices, count);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}