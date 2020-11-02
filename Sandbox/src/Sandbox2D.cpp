#include "Sandbox2D.h"

#include "Mana/Render/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, true)
{
}

void Sandbox2D::onAttach()
{
    m_checkerBoard = Mana::Texture2D::Create("assets/textures/Checkerboard.png");
    m_mario = Mana::Texture2D::Create("assets/textures/mario.png");
    m_spriteSheet = Mana::Texture2D::Create("assets/game/Spritesheet/RPGpack_sheet_2X.png");
    m_marioSpriteSheet = Mana::Texture2D::Create("assets/textures/mario_characters.png");

    m_textureStairs = Mana::SubTexture2D::createFromCoords(m_spriteSheet, { 4, 1 }, { 128, 128 }, { 1, 2 });
    m_smb = Mana::SubTexture2D::createFromCoords(m_marioSpriteSheet, { 1, 8 }, { 8, 16 }, { 1, 1 });


    m_particle.ColorBegin = { 84 / 255.0f, 0 / 255.0f, 168 / 255.0f, 1.0f };
    m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
    m_particle.LifeTime = 5.0f;
    m_particle.Velocity = { 0.0f, 0.0f };
    m_particle.VelocityVariation = { 3.0f, 1.0f };
    m_particle.Position = { 0.0f, 0.0f };

    m_cameraController.setZoomLevel(5.0f);
}

void Sandbox2D::onDetach()
{

}

void Sandbox2D::onUpdate(Mana::TimeStep dt)
{
    m_cameraController.onUpdate(dt);

    Mana::Renderer2D::resetStats();

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

    static float rotation = 0.0f;
    rotation += dt * 20.0f;

    Mana::Renderer2D::beginScene(m_cameraController.getCamera());

    Mana::Renderer2D::drawQuad(glm::vec3(0.0f, 0.0f, -0.2f), { 20.0f, 20.0f }, m_checkerBoard, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    Mana::Renderer2D::drawQuad({ -0.75, 0.5f, 0.0f}, { 0.8f, 0.8f }, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
    Mana::Renderer2D::drawQuad({  0.5f,-0.5f, 0.0f}, { 0.5f, 0.75f }, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
    Mana::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_checkerBoard, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    /*for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            glm::vec3 color = { (x + 5.0f) / 10, 0.2f, (y + 5.0f) / 10 };
            Mana::Renderer2D::drawQuad({ x, y, -0.1}, { 0.45f, 0.45f }, glm::vec4(color, 0.5f));
        }
    }*/

    //Mana::Renderer2D::drawQuad(glm::vec3(0.0f, 0.0f, 0.2f), { 1.0f, 1.0f }, m_spriteSheet, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    Mana::Renderer2D::drawQuad(glm::vec3(0.5f, 0.5f, 0.1f), { 1.0f, 1.0f }, m_textureStairs, 1.0f, {1.0f, 1.0f, 1.0f, 1.0f});
    Mana::Renderer2D::drawQuad(glm::vec3(m_cubePosition, 0.2f), { 1.0f, 1.0f }, m_mario);

    Mana::Renderer2D::endScene();

#if 1
    if (Mana::Input::isMouseButtonPressed(MA_MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = Mana::Input::getMousPos();
        auto width = Mana::Application::get().getWindow().getWidth();
        auto height = Mana::Application::get().getWindow().getHeight();

        auto bounds = m_cameraController.getBounds();
        auto pos = m_cameraController.getCamera().getPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_particle.Position = { x + pos.x, y + pos.y };
        for (int i = 0; i < 5; i++)
            m_particleSystem.Emit(m_particle);
    }
    
    m_particleSystem.OnUpdate(dt);
    m_particleSystem.OnRender(m_cameraController.getCamera());
#endif

}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");

    auto stats = Mana::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quad Count: %d", stats.quadCount);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::ColorPicker4("Square Color Picker", glm::value_ptr(m_Color));

    ImGui::End();
}

void Sandbox2D::onEvent(Mana::Event& event)
{
    m_cameraController.onEvent(event);
}
