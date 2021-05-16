//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLBUFFER_H
#define VIKING_OPENGLBUFFER_H

#include "Viking/Renderer/Buffer.h"

namespace Viking {
    class OpenGLVertexBuffer final: public VertexBuffer {
    public:
        explicit OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        void setData(const void* data, uint32_t size) override;

        [[nodiscard]] const BufferLayout& getLayout() const override {
            return mLayout;
        }

        void setLayout(const BufferLayout& layout) override {
            mLayout = layout;
        }
    private:
        uint32_t mRendererID;
        BufferLayout mLayout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] virtual uint32_t getCount() const {
            return mCount;
        }
    private:
        uint32_t mRendererID;
        uint32_t mCount;
    };
}

#endif //VIKING_OPENGLBUFFER_H
