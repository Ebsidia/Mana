#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace Mana {

    class Renderer2D
    {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicCamera& camera);
        static void endScene();

        static void drawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color);
        static void drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4& color);
        static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4& color);
        static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    };
}