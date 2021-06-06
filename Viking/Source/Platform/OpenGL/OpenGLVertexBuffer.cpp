#include "vipch.h"

#include "Platform/openGL/OpenGLVertexBuffer.h"
#include "Viking/Renderer/Buffer.h"

#include <glad/gl.h>

namespace Viking
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        glCreateBuffers(1, &mRendererId);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, const size_t size)
    {
        mLocalData = Buffer::copy(data, size);

        glCreateBuffers(1, &mRendererId);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
        glBufferData(GL_ARRAY_BUFFER, size, mLocalData.mData, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &mRendererId);
    }

    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
    }

    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
