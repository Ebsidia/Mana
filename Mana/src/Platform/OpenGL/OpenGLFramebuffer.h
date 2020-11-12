#pragma once

#include "Mana/Render/Framebuffer.h"

namespace Mana {

    class OpenGlFramebuffer : public Framebuffer
    {
    public:
        OpenGlFramebuffer(const FramebufferSpecs& spec);
        virtual ~OpenGlFramebuffer();

        void invalidate();

        virtual void bind() override;
        virtual void unbind() override;

        virtual void resize(uint32_t width, uint32_t height) override;

        virtual uint32_t getColorAttachmentRendererId() const override { return m_colorAttachment; }

        virtual const FramebufferSpecs& getSpecification() const override { return m_specification; }

    private:
        uint32_t m_rendererId = 0;
        uint32_t m_colorAttachment = 0, m_depthAttachment = 0;
        FramebufferSpecs m_specification;
    };
}