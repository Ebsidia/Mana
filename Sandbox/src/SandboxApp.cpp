#include "Mana.h"

#include "imgui/imgui.h"

class ExampleLayer : public Mana::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {

    }

    void onUpdate() override
    {
        //MA_INFO("ExampleLayer::Update");
        if(Mana::Input::isKeyPressed(MA_KEY_TAB))
        {
            MA_TRACE("Tab key is pressed");
        }
    }

    virtual void onEvent(Mana::Event& event) override
    {
        //MA_TRACE("{0}", event);
        if (event.getEventType() == Mana::EventType::KeyPressed)
        {
            Mana::KeyPressedEvent& e = (Mana::KeyPressedEvent & )event;
            MA_TRACE("{0}", (char)e.getKeyCode());
        }
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Mana");
        ImGui::End();
    }
};

class Sandbox : public Mana::Application
{
public:
    Sandbox()
    {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

//Required function defined in the Application from Application.h
Mana::Application* Mana::CreateApplication()
{
    return new Sandbox();
}