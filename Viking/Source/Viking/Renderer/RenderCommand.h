//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_RENDERCOMMAND_H
#define VIKING_RENDERCOMMAND_H

#include "Viking/Renderer/RendererAPI.h"

namespace Viking {
    class RenderCommand {
    public:
        static void init() {
            sRendererAPI->init();
        }

        static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            sRendererAPI->setViewport(x, y, width, height);
        }

        static void setClearColor(const glm::vec4& color) {
            sRendererAPI->setClearColor(color);
        }

        static void clear() {
            sRendererAPI->clear();
        }

        static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) {
            sRendererAPI->drawIndexed(vertexArray, count);
        }

    private:
        static Scope<RendererAPI> sRendererAPI;
    };
}

#endif //VIKING_RENDERCOMMAND_H
