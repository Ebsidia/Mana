#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    virtual ~Player();

    virtual void loadAssets() override;
    virtual void onUpdate(Mana::TimeStep dt) override;
    virtual void onRender() override;

    virtual void onImGuiRender() override;

    virtual void reset() override;

    virtual const glm::vec2& getPosition() const override { return m_position; }
    virtual void setPosition(glm::vec2 pos) { m_position = pos; }

    const glm::vec2& getSize() const override { return m_size; }
    void setSize(glm::vec2 size) { m_size = size; }

private:
    glm::vec2 m_size = {100.0f, 20.0f};
    glm::vec2 m_position;
    glm::vec2 m_velocity = { 500.0f, 0.0f };

    Mana::Ref<Mana::Texture2D> m_paddleTexture;

};