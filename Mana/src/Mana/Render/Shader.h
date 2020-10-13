#pragma once

#include "string"

namespace Mana {
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static Shader* Create(const std::string& filePath);
        static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
    };
}