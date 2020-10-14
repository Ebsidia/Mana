#include "mapch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Mana {

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

    void Renderer::init()
    {
        RenderCommand::init();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::setViewPort(0, 0, width, height);
    }

    void Renderer::beginScene(OrthographicCamera& camera)
    {
        m_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {

    }

    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_viewProjection", m_sceneData->viewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_transform", transform);



        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }

}