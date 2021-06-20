//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Viking {

    Ref<Shader> Shader::create(const std::string &filepath) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLShader>(filepath);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
