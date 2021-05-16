//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLRENDERERAPI_H
#define VIKING_OPENGLRENDERERAPI_H

#include "Viking/Renderer/RendererAPI.h"

namespace Viking {
    class OpenGLRendererAPI final: public RendererAPI{
    public:
        void init() override;
        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void setClearColor(const glm::vec4& color) override;
        void clear() override;

        void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
    };
}

#endif //VIKING_OPENGLRENDERERAPI_H
