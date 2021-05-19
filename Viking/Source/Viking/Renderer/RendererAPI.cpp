//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Viking {
    RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;
    
    Scope<RendererAPI> RendererAPI::create() {
        switch (sAPI) {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createScope<OpenGLRendererAPI>();
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
