#include "Bricks.h"

Bricks::Bricks(const char* filePath)
    : m_filePath(filePath)
{
    
}

Bricks::~Bricks()
{

}

void Bricks::loadAssets()
{
    m_brickTexture = Mana::Texture2D::Create(m_filePath);
}

void Bricks::onUpdate(Mana::TimeStep dt)
{

}

void Bricks::onRender()
{
    Mana::Renderer2D::drawQuad(m_position, m_size, m_brickTexture, 1.0f, m_color);
}

void Bricks::onImGuiRender()
{

}

void Bricks::reset()
{

}

