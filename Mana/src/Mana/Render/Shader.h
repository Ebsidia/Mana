#pragma once

#include "string"
#include <glm/glm.hpp>

namespace Mana {
    class Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void bind();
        void unbind();

        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        unsigned int m_rendererID;
    };
}