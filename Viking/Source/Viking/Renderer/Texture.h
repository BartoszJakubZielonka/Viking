//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_TEXTURE_H
#define VIKING_TEXTURE_H

#include <string>
#include "Viking/Core/Base.h"

namespace Viking {
    class Texture {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;

        virtual void setData(void* data, uint32_t size) = 0;
        virtual void bind(uint32_t slot = 0) = 0;

        virtual bool operator==(const Texture& other) const = 0;
    };

    class Texture2D: public Texture {
    public:
        [[nodiscard]] static Ref<Texture2D> create(uint32_t width, uint32_t height);
        [[nodiscard]] static Ref<Texture2D> create(const std::string& path);
    };
}

#endif //VIKING_TEXTURE_H
