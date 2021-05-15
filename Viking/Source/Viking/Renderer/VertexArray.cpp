//
// Created by batzi on 15.05.2021.
//

#include "Viking/Source/Viking/Renderer/VertexArray.h"
namespace Viking {
    Ref <VertexArray> Viking::VertexArray::create() {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexArray>();
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}