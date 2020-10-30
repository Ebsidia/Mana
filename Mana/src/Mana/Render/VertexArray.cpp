#include "mapch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mana {

    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexArray>();
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

}