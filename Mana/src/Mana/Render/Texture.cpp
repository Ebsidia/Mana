#include "mapch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Mana {

    Ref<Texture2D> Texture2D::Create(const std::string& filePath)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(filePath);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

    Ref<Mana::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(width, height);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

}