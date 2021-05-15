//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Viking/Renderer/GraphicsContext.h"

#include "Viking/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Viking {

    Scope<GraphicsContext> GraphicsContext::create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}