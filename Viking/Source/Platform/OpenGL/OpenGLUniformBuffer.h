//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLUNIFORMBUFFER_H
#define VIKING_OPENGLUNIFORMBUFFER_H

#include "Viking/Renderer/UniformBuffer.h"

namespace Viking {
    class OpenGLUniformBuffer final: public UniformBuffer {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        ~OpenGLUniformBuffer() override;

        void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

    private:
        uint32_t mRendererID = 0;
    };
}

#endif //VIKING_OPENGLUNIFORMBUFFER_H
