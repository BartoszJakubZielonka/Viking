//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLVERTEXARRAY_H
#define VIKING_OPENGLVERTEXARRAY_H

#include "Viking/Renderer/VertexArray.h"

namespace Viking {
    class OpenGLVertexArray: public VertexArray{
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        [[nodiscard]] const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override {
            return mVertexBuffers;
        }

        [[nodiscard]] const Ref<IndexBuffer>& getIndexBuffer() const override {
            return mIndexBuffer;
        }

    private:
        uint32_t mRendererID;
        uint32_t mVertexBufferIndex{0};
        std::vector<Ref<VertexBuffer>> mVertexBuffers;
        Ref<IndexBuffer> mIndexBuffer;
    };
}

#endif //VIKING_OPENGLVERTEXARRAY_H
