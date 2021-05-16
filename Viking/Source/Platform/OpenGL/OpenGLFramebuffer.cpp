//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Viking {
    static const uint32_t sMaxFramebufferSize = 8192;

    namespace Utils {
        static GLenum textureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void createTextures(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void bindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool isDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }

        static GLenum VikingFBTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
                case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }

            VI_CORE_ASSERT(false);
            return 0;
        }
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &spec): mSpecification(spec) {
        for (auto spec : mSpecification.Attachments.Attachments)
        {
            if (!Utils::isDepthFormat(spec.TextureFormat))
                mColorAttachmentSpecifications.emplace_back(spec);
            else
                mDepthAttachmentSpecification = spec;
        }

        invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer() {
        glDeleteFramebuffers(1, &mRendererID);
        glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
        glDeleteTextures(1, &mDepthAttachment);
    }

    void OpenGLFramebuffer::invalidate() {
        if (mRendererID)
        {
            glDeleteFramebuffers(1, &mRendererID);
            glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
            glDeleteTextures(1, &mDepthAttachment);

            mColorAttachments.clear();
            mDepthAttachment = 0;
        }

        glCreateFramebuffers(1, &mRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

        bool multisample = mSpecification.Samples > 1;

        // Attachments
        if (mColorAttachmentSpecifications.size())
        {
            mColorAttachments.resize(mColorAttachmentSpecifications.size());
            Utils::createTextures(multisample, mColorAttachments.data(), mColorAttachments.size());

            for (size_t i = 0; i < mColorAttachments.size(); i++)
            {
                Utils::bindTexture(multisample, mColorAttachments[i]);
                switch (mColorAttachmentSpecifications[i].TextureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::attachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_RGBA8, GL_RGBA, mSpecification.Width, mSpecification.Height, i);
                        break;
                    case FramebufferTextureFormat::RED_INTEGER:
                        Utils::attachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_R32I, GL_RED_INTEGER, mSpecification.Width, mSpecification.Height, i);
                        break;
                }
            }
        }

        if (mDepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            Utils::createTextures(multisample, &mDepthAttachment, 1);
            Utils::bindTexture(multisample, mDepthAttachment);
            switch (mDepthAttachmentSpecification.TextureFormat)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::attachDepthTexture(mDepthAttachment, mSpecification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mSpecification.Width, mSpecification.Height);
                    break;
            }
        }

        if (mColorAttachments.size() > 1)
        {
            HZ_CORE_ASSERT(mColorAttachments.size() <= 4);
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(mColorAttachments.size(), buffers);
        }
        else if (mColorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

        VI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, mSpecification.Width, mSpecification.Height);
    }

    void OpenGLFramebuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0 || width > sMaxFramebufferSize || height > sMaxFramebufferSize)
        {
            VI_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
            return;
        }
        mSpecification.Width = width;
        mSpecification.Height = height;

        invalidate();
    }

    int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
        VI_CORE_ASSERT(attachmentIndex < mColorAttachments.size());

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
        VI_CORE_ASSERT(attachmentIndex < mColorAttachments.size());

        auto& spec = mColorAttachmentSpecifications[attachmentIndex];
        glClearTexImage(mColorAttachments[attachmentIndex], 0, Utils::VikingFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }
}