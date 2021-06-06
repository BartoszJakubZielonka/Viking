#pragma once

#include "Viking/Renderer/Buffer.h"
#include "Viking/Renderer/IndexBuffer.h"

namespace Viking
{
    class OpenGLIndexBuffer final : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(void* data, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] uint32_t getCount() const override;
    private:
        Buffer mBuffer;
        uint32_t mRendererID;
        uint32_t mCount;
    };
}
