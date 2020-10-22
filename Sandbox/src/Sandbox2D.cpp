#include "Sandbox2D.h"

#include "Mana/Render/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, 450.0f,  true)
{
}

void Sandbox2D::onAttach()
{
    m_checkerBoard = Mana::Texture2D::Create("assets/textures/Checkerboard.png");
    m_mario = Mana::Texture2D::Create("assets/textures/mario.png");
}

void Sandbox2D::onDetach()
{

}

void Sandbox2D::onUpdate(Mana::TimeStep dt)
{
    m_cameraController.onUpdate(dt);


    if (Mana::Input::isKeyPressed(MA_KEY_LEFT))
    {
        m_cubePosition.x -= m_cubeMoveSpeed * dt;
    }
    else if (Mana::Input::isKeyPressed(MA_KEY_RIGHT))
    {
        m_cubePosition.x += m_cubeMoveSpeed * dt;
    }

    if (Mana::Input::isKeyPressed(MA_KEY_UP))
    {
        m_cubePosition.y += m_cubeMoveSpeed * dt;
    }
    else if (Mana::Input::isKeyPressed(MA_KEY_DOWN))
    {
        m_cubePosition.y -= m_cubeMoveSpeed * dt;
    }

    Mana::RenderCommand::setClearColor({ 0.15f, 0.15f, 0.15f, 1 });
    Mana::RenderCommand::clear();

    Mana::Renderer2D::beginScene(m_cameraController.getCamera());

    Mana::Renderer2D::drawRotatedQuad({ -0.75, 0.5f }, { 0.8f, 0.8f }, glm::radians(45.0f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
    Mana::Renderer2D::drawQuad({  0.5f,-0.5f }, { 0.5f, 0.75f }, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
    Mana::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerBoard, 10.0f);
    Mana::Renderer2D::drawQuad(glm::vec3(m_cubePosition, 0.1f), { 64.0f, 64.0f }, m_mario, 1.0f, m_Color);

    Mana::Renderer2D::endScene();

}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");

    ImGui::ColorPicker4("Square Color Picker", glm::value_ptr(m_Color));

    ImGui::End();
}

void Sandbox2D::onEvent(Mana::Event& event)
{
    m_cameraController.onEvent(event);
}
