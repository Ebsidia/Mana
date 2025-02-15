#pragma once

#include "Mana/Core/core.h"

namespace Mana {

    struct FramebufferSpecs
    {
        uint32_t width, height;
        
        uint32_t samples = 1;


        bool swapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t getColorAttachmentRendererId() const = 0;

        virtual const FramebufferSpecs& getSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecs& spec);
    };
}