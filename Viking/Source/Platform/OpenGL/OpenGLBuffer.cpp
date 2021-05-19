//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/gl.h>

namespace Viking {

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        VI_PROFILE_FUNCTION();

        glCreateBuffers(1, &mRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size) {
        VI_PROFILE_FUNCTION();

        glCreateBuffers(1, &mRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        VI_PROFILE_FUNCTION();

        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        VI_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        VI_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void *data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count): mCount(count) {
        VI_PROFILE_FUNCTION();

        glCreateBuffers(1, &mRendererID);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        VI_PROFILE_FUNCTION();

        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLIndexBuffer::bind() const {
        VI_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        VI_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
