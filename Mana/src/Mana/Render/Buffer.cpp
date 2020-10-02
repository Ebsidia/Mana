#include "mapch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mana { 

    VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
            case RendererAPI::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::OpenGL:   return new OpenGLIndexBuffer(indices, count);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

}