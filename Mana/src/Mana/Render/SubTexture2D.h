#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Mana {

    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        const Ref<Texture2D> getTexture() const { return m_texture; }
        const glm::vec2* getTextureCoords() const { return m_textureCoords; }

        static Ref<SubTexture2D> createFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
    private:
        Ref<Texture2D> m_texture;

        glm::vec2 m_textureCoords[4];
    };
}