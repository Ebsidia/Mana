#include "mapch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Mana {

    Shader* Shader::Create(const std::string& filePath)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLShader(filePath);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLShader(vertexSource, fragmentSource);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

}