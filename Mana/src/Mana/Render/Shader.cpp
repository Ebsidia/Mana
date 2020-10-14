#include "mapch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Mana {

    Ref<Shader> Shader::Create(const std::string& filePath)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(filePath);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:     MA_CORE_ASSERT(false, "RendererAPI::None is currently not support"); return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
        }

        MA_CORE_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }

    void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
    {

        MA_CORE_ASSERT(!exists(name), "Shader already exists!");

        m_shaders[name] = shader;
    }

    void ShaderLibrary::add(const Ref<Shader>& shader)
    {
        auto& name = shader->getName();

        add(name, shader);
    }

    Ref<Mana::Shader> ShaderLibrary::load(const std::string& filePath)
    {
        auto shader = Shader::Create(filePath);
        add(shader);
        return shader;
    }

    Ref<Mana::Shader> ShaderLibrary::load(const std::string& name, const std::string& filePath)
    {
        auto shader = Shader::Create(filePath);
        add(name, shader);
        return shader;
    }

    Ref<Mana::Shader> ShaderLibrary::get(const std::string& name)
    {
        MA_CORE_ASSERT(exists(name), "Shader not found!");
        return m_shaders[name];
    }

    bool ShaderLibrary::exists(const std::string& name) const
    {
        return m_shaders.find(name) != m_shaders.end();
    }

}