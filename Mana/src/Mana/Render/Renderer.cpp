#include "mapch.h"
#include "Renderer.h"

namespace Mana {

    

    void Renderer::beginScene()
    {

    }

    void Renderer::endScene()
    {

    }

    void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

}