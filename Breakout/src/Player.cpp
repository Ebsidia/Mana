#include "Player.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

Player::Player()
{

}

Player::~Player()
{

}

void Player::loadAssets()
{
    m_paddleTexture = Mana::Texture2D::Create("assets/Breakout/textures/paddle.png");
}

void Player::onUpdate(Mana::TimeStep dt)
{
    if (Mana::Input::isKeyPressed(MA_KEY_A))
    {
        if (m_position.x >= 0.0f + m_size.x / 2)
        {
            m_position.x -= m_velocity.x * dt;
        }
    } 

    if (Mana::Input::isKeyPressed(MA_KEY_D))
    {
        if (m_position.x <= 1600 - m_size.x / 2)
            m_position.x += m_velocity.x * dt;
    }
}

void Player::onRender()
{
    Mana::Renderer2D::drawQuad(m_position, m_size, m_paddleTexture);
}

void Player::onImGuiRender()
{
    
}

void Player::reset()
{
    m_position = { 900.0f, 700.0f };
    m_velocity = { 4.0f, 0.0f };
}

