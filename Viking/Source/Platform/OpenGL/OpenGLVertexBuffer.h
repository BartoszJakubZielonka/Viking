#pragma once

#include "Viking/Renderer/Buffer.h"
#include "Viking/Renderer/VertexBuffer.h"

namespace Viking
{
    class OpenGLVertexBuffer final : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(void* data, size_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] const BufferLayout& getLayout() const override { return mLayout; }
        void setLayout(const BufferLayout& l) override { mLayout = l; }
        void setData(const void* data, uint32_t size) override;
    private:
        Buffer mLocalData;
        BufferLayout mLayout;
        uint32_t mRendererId;
    };
}
