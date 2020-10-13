#pragma once

#include <memory>
#include "Mana/Render/Buffer.h"

//Renderer API agnostic interface
namespace Mana{

    class VertexArray 
    {
    public:
        virtual ~VertexArray() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
  
        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}