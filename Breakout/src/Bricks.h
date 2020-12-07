#pragma once

#include "GameObject.h"

class Bricks : public GameObject
{
public:
    Bricks(const char* filePath);
    virtual ~Bricks();

    virtual void loadAssets() override;
    virtual void onUpdate(Mana::TimeStep dt) override;
    virtual void onRender() override;

    virtual void onImGuiRender() override;

    virtual void reset() override;

    virtual const glm::vec2& getPosition() const override { return m_position; }
    virtual const glm::vec2& getSize() const override { return m_size; }
    const bool isDestroyed() const { return m_destroyed; }
    const bool isSolid() const { return m_solid; }

    void setPosition(glm::vec2 pos) { m_position = pos; }
    void setDestroyed(bool destroyed) { m_destroyed = destroyed; }
    void setSolid(bool solid) { m_solid = solid; }
    void setSize(glm::vec2 size) { m_size = size; }
    void setColor(glm::vec4 color) { m_color = color; }

private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec4 m_color;

    bool m_solid = false;
    bool m_destroyed = false;

    Mana::Ref<Mana::Texture2D> m_brickTexture;
    const char* m_filePath;
};