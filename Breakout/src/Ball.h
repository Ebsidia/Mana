#pragma once

#include "GameObject.h"


class Ball : public GameObject
{
public:
    Ball();
    virtual ~Ball();

    virtual void loadAssets() override;
    virtual void onUpdate(Mana::TimeStep dt) override;
    virtual void onRender() override;

    virtual void onImGuiRender() override;

    virtual void reset() override;

    void setIsStuck(bool stuck) { m_isStuck = stuck; }
    bool getIsStuck() { return m_isStuck; }

    void setPosition(glm::vec2 pos) { m_position = pos; }
    virtual const glm::vec2& getPosition() const  override { return m_position; }

    void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
    const glm::vec2& getVelocity() const { return m_velocity; }

    virtual const glm::vec2& getSize() const override { return m_size; }
    void setSize(glm::vec2 size) { m_size = size; }

    float const getRadius() const { return m_radius; }

    void getPaddlePos(); 

private:
    float m_radius = 12.5;
    glm::vec2 m_size = { m_radius * 2, m_radius * 2 };
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    

    bool m_isStuck = true;

    Mana::Ref<Mana::Texture2D> m_ballTexture;
};