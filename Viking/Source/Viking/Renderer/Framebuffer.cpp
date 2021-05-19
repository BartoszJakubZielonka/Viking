//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Framebuffer.h"

#include "Viking/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Viking {
    Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLFramebuffer>(spec);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
