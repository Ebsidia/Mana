#include "GameLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm//gtc/type_ptr.hpp>

GameLayer::GameLayer()
    : Layer("GameLayer")
{
    auto& window = Mana::Application::get().getWindow();
    createCamera(window.getWidth(), window.getHeight());
}

void GameLayer::onAttach()
{
    m_scene.init();

    
}

void GameLayer::onDetach()
{

}

void GameLayer::onUpdate(Mana::TimeStep dt)
{

    m_scene.onUpdate(dt);

    //render the game
    Mana::RenderCommand::setClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
    Mana::RenderCommand::clear();

    Mana::Renderer2D::beginScene(*m_camera);
    m_scene.onRender();
    Mana::Renderer2D::endScene();
}

void GameLayer::onImGuiRender()
{
    m_scene.onImGuiRender();
}

void GameLayer::onEvent(Mana::Event& event)
{
    Mana::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Mana::WindowResizeEvent>(MA_BIND_EVENT_FN(GameLayer::onWindowResize));

    m_scene.onEvent(event);
}

bool GameLayer::onWindowResize(Mana::WindowResizeEvent& event)
{
    createCamera(event.getWidth(), event.getHeight());
    return false;
}

void GameLayer::createCamera(uint32_t width, uint32_t height)
{
    float aspectRatio = (float)width / (float)height;

    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    m_camera = Mana::CreateScope<Mana::OrthographicCamera>(0, 1600, 900, 0);
}

