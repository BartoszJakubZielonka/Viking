//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_FRAMEBUFFER_H
#define VIKING_FRAMEBUFFER_H

#include "Viking/Core/Base.h"

namespace Viking {
    enum class FramebufferTextureFormat {
        None = 0,

        //Color
        RGBA8,
        RED_INTEGER,

        //Depth/Stencil
        DEPTH24STENCIL8,

        //Defaults
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification {
        FramebufferTextureSpecification() = default;
        explicit FramebufferTextureSpecification(FramebufferTextureFormat format): TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentSpecification {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments): Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification {
        uint32_t Width{0}, Height{0};
        FramebufferAttachmentSpecification Attachments;
        uint32_t Samples{1};

        bool SwapChainTarget{false};
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;
        virtual void readPixel(uint32_t attachmentIndex, int x, int y) const = 0;

        virtual void clearAttachment(uint32_t index = 0) const = 0;

        [[nodiscard]] virtual uint32_t getColorAttachmentRendererId(uint32_t index = 0) const = 0;

        [[nodiscard]] virtual const FramebufferSpecification& getSpecification() const = 0;

        [[nodiscard]] static Ref<Framebuffer> create(const FramebufferSpecification& spec);
    };
}

#endif //VIKING_FRAMEBUFFER_H
