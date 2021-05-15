//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_UNIFORMBUFFER_H
#define VIKING_UNIFORMBUFFER_H

#include "Viking/Core/Base.h"

namespace Viking {
    class UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;
        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        static Ref<UniformBuffer> create(uint32_t size, uint32_t binding);
    };
}

#endif //VIKING_UNIFORMBUFFER_H
