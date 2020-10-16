#include "mapch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "shader.h"
#include "RenderCommand.h"

namespace Mana {

    struct renderer2DStorage
    {
        Ref<VertexArray> vertexArray;
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;
    };

    static renderer2DStorage* s_renderData;

    void Renderer2D::init()
    {
        s_renderData = new renderer2DStorage();

        s_renderData->vertexArray = VertexArray::Create();

        float square[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Mana::Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(square, sizeof(square)));

        squareVB->setLayout({
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float2, "a_texCoord"},
            });

        s_renderData->vertexArray->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        Mana::Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(square) / sizeof(uint32_t)));
        s_renderData->vertexArray->setIndexBuffer(squareIB);

        s_renderData->whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_renderData->whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

        s_renderData->textureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_renderData->textureShader->bind();
        s_renderData->textureShader->setInt("u_texture", 0);
    }

    void Renderer2D::shutdown()
    {
        delete s_renderData;
    }

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        s_renderData->textureShader->bind();
        s_renderData->textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());
    }

    void Renderer2D::endScene()
    {

    }

    /////////////////Draw Quads////////////////////////////////////////////////

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color)
    {
        s_renderData->textureShader->setFloat4("u_color", color);
        s_renderData->textureShader->setFloat("u_tilingFactor", 1.0f);
        s_renderData->whiteTexture->bind();


        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        s_renderData->textureShader->setMat4("u_transform", transform);

        s_renderData->vertexArray->bind();
        RenderCommand::drawIndexed(s_renderData->vertexArray);
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        s_renderData->textureShader->setFloat4("u_color", tintColor);
        s_renderData->textureShader->setFloat("u_tilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_renderData->textureShader->setMat4("u_transform", transform);

        s_renderData->vertexArray->bind();
        RenderCommand::drawIndexed(s_renderData->vertexArray);
    }

    /////////////////Draw Rotated Quads////////////////////////////////////////////////

    void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4& color)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4& color)
    {
        s_renderData->textureShader->setFloat4("u_color", color);
        s_renderData->textureShader->setFloat("u_tilingFactor", 1.0f);
        s_renderData->whiteTexture->bind();


        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_renderData->textureShader->setMat4("u_transform", transform);

        s_renderData->vertexArray->bind();
        RenderCommand::drawIndexed(s_renderData->vertexArray);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        s_renderData->textureShader->setFloat4("u_color", tintColor);
        s_renderData->textureShader->setFloat("u_tilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_renderData->textureShader->setMat4("u_transform", transform);

        s_renderData->vertexArray->bind();
        RenderCommand::drawIndexed(s_renderData->vertexArray);
    }

}