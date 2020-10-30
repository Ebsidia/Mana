#include "mapch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "shader.h"
#include "RenderCommand.h"

namespace Mana {

    struct quadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct renderer2DData
    {
        static const uint32_t maxQuads = 20000;
        static const uint32_t maxVertices = maxQuads * 4;
        static const uint32_t maxIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 32;

        Ref<VertexArray> vertexArray;
        Ref<VertexBuffer> vertexBuffer;
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        quadVertex* quadVertexBufferBase = nullptr;
        quadVertex* quadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1;

        glm::vec4 quadVertexPosition[4];

        Renderer2D::Statistics stats;

    };

    static renderer2DData s_renderData;

    

    void Renderer2D::init()
    {
        s_renderData.vertexArray = VertexArray::Create();

        s_renderData.vertexBuffer = VertexBuffer::Create(s_renderData.maxVertices * sizeof(quadVertex));

        s_renderData.vertexBuffer->setLayout({
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float4, "a_color"},
            {ShaderDataType::Float2, "a_texCoord"},
            {ShaderDataType::Float,  "a_texIndex"},
            {ShaderDataType::Float,  "a_tilingFactor"}
            });

        s_renderData.vertexArray->addVertexBuffer(s_renderData.vertexBuffer);

        s_renderData.quadVertexBufferBase = new quadVertex[s_renderData.maxVertices];

        uint32_t* quadIndices = new uint32_t[s_renderData.maxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_renderData.maxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_renderData.maxIndices);
        s_renderData.vertexArray->setIndexBuffer(quadIB);

        delete[] quadIndices;

        s_renderData.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_renderData.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

        int32_t samplers[s_renderData.maxTextureSlots];
        for (uint32_t i = 0; i < s_renderData.maxTextureSlots; i++)
            samplers[i] = i;

        s_renderData.textureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_renderData.textureShader->bind();
        s_renderData.textureShader->setIntArray("u_texture", samplers, s_renderData.maxTextureSlots);


        s_renderData.textureSlots[0] = s_renderData.whiteTexture;

        s_renderData.quadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_renderData.quadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_renderData.quadVertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_renderData.quadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::shutdown()
    {
    }

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        s_renderData.textureShader->bind();
        s_renderData.textureShader->setMat4("u_viewProjection", camera.getViewProjectionMatrix());

        s_renderData.quadIndexCount = 0;
        s_renderData.quadVertexBufferPtr = s_renderData.quadVertexBufferBase;

        s_renderData.textureSlotIndex = 1;
    }

    void Renderer2D::endScene()
    {
        uint32_t dataSize = (uint8_t*)s_renderData.quadVertexBufferPtr - (uint8_t*)s_renderData.quadVertexBufferBase;
        s_renderData.vertexBuffer->setData(s_renderData.quadVertexBufferBase, dataSize);

        flush();
    }

    void Renderer2D::flush()
    {
        for (uint32_t i = 0; i < s_renderData.textureSlotIndex; i++)
            s_renderData.textureSlots[i]->bind(i);

        RenderCommand::drawIndexed(s_renderData.vertexArray, s_renderData.quadIndexCount);

        s_renderData.stats.drawCalls++;
    }

    void Renderer2D::startNewBatch()
    {
        endScene();

        s_renderData.quadIndexCount = 0;
        s_renderData.quadVertexBufferPtr = s_renderData.quadVertexBufferBase;

        s_renderData.textureSlotIndex = 1;
    }

    /////////////////Draw Quads////////////////////////////////////////////////

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color)
    {
        if (s_renderData.quadIndexCount >= renderer2DData::maxIndices)
            startNewBatch();
        

        //white texture
        const float texIndex = 0;
        const float tilingFactor = 1;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        //bottom left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[0];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //bottom right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[1];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[2];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[3];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        s_renderData.quadIndexCount += 6;

        s_renderData.stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        if (s_renderData.quadIndexCount >= renderer2DData::maxIndices)
            startNewBatch();

        constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_renderData.textureSlotIndex; i++)
        {
            if (*s_renderData.textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_renderData.textureSlotIndex;
            s_renderData.textureSlots[s_renderData.textureSlotIndex] = texture;
            s_renderData.textureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        //bottom left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[0];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //bottom right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[1];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[2];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[3];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        s_renderData.quadIndexCount += 6;

        s_renderData.stats.quadCount++;
    }

    /////////////////Draw Rotated Quads////////////////////////////////////////////////

    void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4& color)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4& color)
    {
        if (s_renderData.quadIndexCount >= renderer2DData::maxIndices)
            startNewBatch();

        const float texIndex = 0;
        const float tilingFactor = 1;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        //bottom left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[0];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //bottom right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[1];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[2];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[3];
        s_renderData.quadVertexBufferPtr->color = color;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = texIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        s_renderData.quadIndexCount += 6;

        s_renderData.stats.quadCount++;
    }

    void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        if (s_renderData.quadIndexCount >= renderer2DData::maxIndices)
            startNewBatch();

        constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_renderData.textureSlotIndex; i++)
        {
            if (*s_renderData.textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_renderData.textureSlotIndex;
            s_renderData.textureSlots[s_renderData.textureSlotIndex] = texture;
            s_renderData.textureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        //bottom left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[0];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //bottom right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[1];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top right
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[2];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        //top left
        s_renderData.quadVertexBufferPtr->position = transform * s_renderData.quadVertexPosition[3];
        s_renderData.quadVertexBufferPtr->color = tintColor;
        s_renderData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_renderData.quadVertexBufferPtr->texIndex = textureIndex;
        s_renderData.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_renderData.quadVertexBufferPtr++;

        s_renderData.quadIndexCount += 6;

        s_renderData.stats.quadCount++;
    }

    Renderer2D::Statistics Renderer2D::getStats()
    {
        return s_renderData.stats;
    }

    void Renderer2D::resetStats()
    {
        memset(&s_renderData.stats, 0, sizeof(Statistics));
        
    }

}