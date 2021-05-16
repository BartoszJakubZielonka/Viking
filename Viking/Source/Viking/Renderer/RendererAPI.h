//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_RENDERERAPI_H
#define VIKING_RENDERERAPI_H

#include <glm/glm.hpp>

#include "Viking/Renderer/VertexArray.h"

namespace Viking {
    class RendererAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };

        virtual ~RendererAPI() = default;

        virtual void init() = 0;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t heigth) = 0;
        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        static API getAPI() {
            return sAPI;
        }
        static Scope<RendererAPI> create();

    private:
        static API sAPI;
    };
}

#endif //VIKING_RENDERERAPI_H
