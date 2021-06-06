#include "vipch.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

#include "glad/gl.h"
#include "Viking/Renderer/Buffer.h"

namespace Viking
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count) : mCount(count)
    {
        mBuffer = Buffer::copy(data, count);

        glCreateBuffers(1, &mRendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBuffer.mSize, mBuffer.mData, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::getCount() const
    {
        return mBuffer.mSize;
    }
}
