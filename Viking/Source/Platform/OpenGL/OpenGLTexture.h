//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLTEXTURE_H
#define VIKING_OPENGLTEXTURE_H

#include "Viking/Renderer/Texture.h"

#include <glad/gl.h>

namespace Viking {
    class OpenGLTexture2D final: public Texture2D {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        uint32_t getWidth() const override {
            return mWidth;
        }

        uint32_t getHeight() const override {
            return mHeight;
        }

        uint32_t getRendererID() const override {
            return mRendererID;
        }

        void setData(void* data, uint32_t size) override;

        void bind(uint32_t slot = 0) const override;

        bool operator==(const Texture& other) const override
        {
            return mRendererID == ((OpenGLTexture2D&)other).mRendererID;
        }

    private:
        std::string mPath;
        uint32_t mWidth, mHeight;
        uint32_t mRendererID;
        GLenum mInternalFormat, mDataFormat;
    };
}

#endif //VIKING_OPENGLTEXTURE_H
