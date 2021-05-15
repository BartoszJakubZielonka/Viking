//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_VERTEXARRAY_H
#define VIKING_VERTEXARRAY_H

#include <memory>
#include "Viking/Renderer/Buffer.h"

namespace Viking {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
        [[nodiscard]] virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        [[nodiscard]] static Ref<VertexArray> create();
    };
}

#endif //VIKING_VERTEXARRAY_H
