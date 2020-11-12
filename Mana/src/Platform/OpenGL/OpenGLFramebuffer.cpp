#include "mapch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Mana {

    static const uint32_t s_maxFramebufferSize = 8192;
    
    OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecs& spec)
        : m_specification(spec)
    {
        invalidate();
    }

    OpenGlFramebuffer::~OpenGlFramebuffer()
    {
        glDeleteFramebuffers(1, &m_rendererId);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }

    void OpenGlFramebuffer::invalidate()
    {
        if (m_rendererId)
        {
            glDeleteFramebuffers(1, &m_rendererId);
            glDeleteTextures(1, &m_colorAttachment);
            glDeleteTextures(1, &m_depthAttachment);
        }

        glCreateFramebuffers(1, &m_rendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        MA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
        glViewport(0, 0, m_specification.width, m_specification.height);
    }

    void OpenGlFramebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
        {
            MA_CORE_WARN("Invalid Framebuffer size: {0}, {1}", width, height);
            return;
        }

        m_specification.width = width;
        m_specification.height = height;

        invalidate();
    }

}