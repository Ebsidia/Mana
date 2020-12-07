#include "Ball.h"

Ball::Ball()
{

}

Ball::~Ball()
{

}

void Ball::loadAssets()
{
    m_ballTexture = Mana::Texture2D::Create("assets/Breakout/textures/pongball.png");
}

void Ball::onUpdate(Mana::TimeStep dt)
{
    if (Mana::Input::isKeyPressed(MA_KEY_SPACE))
        m_isStuck = false;


    if (!m_isStuck)
    {
        glm::vec2 velocity = { m_velocity.x * dt, m_velocity.y * dt };
        m_position += velocity;

        //setPosition({ m_velocity.x * dt, m_velocity.y * dt });
        // Then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (getPosition().x <= 0.0f)
        {
            m_velocity.x = -m_velocity.x;
            setPosition({0, getPosition().y });
        }
        else if (getPosition().x + m_size.x >= 1600)
        {
            m_velocity.x = -m_velocity.x;
            setPosition({ 1600 - m_size.x, getPosition().y });
        }

        if (getPosition().y <= 0.0f)
        {
            m_velocity.y = -this->m_velocity.y;
            setPosition({getPosition().x, 0});
        }
    }

}

void Ball::onRender()
{
    Mana::Renderer2D::drawQuad({m_position.x, m_position.y, 0.1f}, m_size, m_ballTexture, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Ball::onImGuiRender()
{

}

void Ball::reset()
{

}
