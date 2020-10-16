#pragma once

#include "Mana/Render/Shader.h"
#include "glm/glm.hpp"

//temporary
typedef unsigned int GLenum;

namespace Mana {
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filePath);
        OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        virtual ~OpenGLShader();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void setInt(const std::string& name, int value) override;
        virtual void setFloat(const std::string& name, float value) override;
        virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void setMat4(const std::string& name, const glm::mat4& value) override;

        virtual const std::string& getName() const override { return m_name; }

        void uploadUniformInt(const std::string& name, int values);

        void uploadUniformFloat(const std::string& name, float values);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        std::string readFile(const std::string& filePath);
        std::unordered_map<GLenum, std::string> prePocess(const std::string& source);
        void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        unsigned int m_rendererID;
        std::string m_name;
    };
}