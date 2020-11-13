#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Mana {

    class Renderer2D
    {
    public:
        
        static void init();
        static void shutdown();

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void beginScene(const OrthographicCamera& camera);
        static void endScene();
        static void flush();

        static void drawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color);
        static void drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void drawQuad(const glm::mat4& transform, glm::vec4& color);
        static void drawQuad(const glm::mat4& transform, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4& color);
        static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4& color);
        static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        struct Statistics
        {
            uint32_t drawCalls = 0;
            uint32_t quadCount = 0;

            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static Statistics getStats();
        static void resetStats();

    private:
        static void startNewBatch();
    };
}