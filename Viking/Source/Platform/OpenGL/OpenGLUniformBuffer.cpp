//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/gl.h>

namespace Viking {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &mRendererID);
        glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, mRendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &mRendererID);
    }


    void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(mRendererID, offset, size, data);
    }
}
