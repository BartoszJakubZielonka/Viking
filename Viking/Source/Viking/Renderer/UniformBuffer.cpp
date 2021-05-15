//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "UniformBuffer.h"

namespace Viking {

    Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLUniformBuffer>(size, binding);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}