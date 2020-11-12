#include "mapch.h"
#include "Framebuffer.h"

#include "Mana/Render/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Mana {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& spec)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGlFramebuffer>(spec);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
    
}