#include "mapch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Mana {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Mana::ShaderDataType::Float:   return GL_FLOAT;
        case Mana::ShaderDataType::Float2:  return GL_FLOAT;
        case Mana::ShaderDataType::Float3:  return GL_FLOAT;
        case Mana::ShaderDataType::Float4:  return GL_FLOAT;
        case Mana::ShaderDataType::Mat3:    return GL_FLOAT;
        case Mana::ShaderDataType::Mat4:    return GL_FLOAT;
        case Mana::ShaderDataType::Int:     return GL_INT;
        case Mana::ShaderDataType::Int2:    return GL_INT;
        case Mana::ShaderDataType::Int3:    return GL_INT;
        case Mana::ShaderDataType::Int4:    return GL_INT;
        case Mana::ShaderDataType::Bool:    return GL_BOOL;
        }

        MA_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        MA_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertexbuffer has no layout");

        glBindVertexArray(m_rendererID);
        vertexBuffer->bind();


        uint32_t index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.getComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)element.Offset);
            index++;
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_rendererID);
        indexBuffer->bind();

        m_indexBuffer = indexBuffer;

    }

}