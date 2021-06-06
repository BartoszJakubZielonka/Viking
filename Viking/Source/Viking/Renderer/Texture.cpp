//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Texture.h"

#include "Viking/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Viking {

    Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLTexture2D>(width, height);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }

    Ref<Texture2D> Texture2D::create(const std::string &path) {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                VI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLTexture2D>(path);
        }

        VI_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
