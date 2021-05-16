//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLFRAMEBUFFER_H
#define VIKING_OPENGLFRAMEBUFFER_H

#include "Viking/Renderer/Framebuffer.h"

namespace Viking {
    class OpenGLFramebuffer final : public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer() override;

        void invalidate();

        void bind() override;
        void unbind() override;

        void resize(uint32_t width, uint32_t height) override;
        int readPixel(uint32_t attachmentIndex, int x, int y) override;

        void clearAttachment(uint32_t attachmentIndex, int value) override;

        uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override {
            VI_CORE_ASSERT(index < mColorAttachments.size());
            return mColorAttachments[index];
        }

        virtual const FramebufferSpecification& getSpecification() const override {
            return mSpecification;
        }
    private:
        uint32_t mRendererID{0};
        FramebufferSpecification mSpecification;

        std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
        FramebufferTextureSpecification mDepthAttachmentSpecification{FramebufferTextureFormat::None};

        std::vector<uint32_t> mColorAttachments;
        uint32_t mDepthAttachment{0};
    };
}

#endif //VIKING_OPENGLFRAMEBUFFER_H
