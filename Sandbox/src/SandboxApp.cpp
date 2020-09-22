#include "Mana.h"

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
    }

    void onEvent(Mana::Event& event) override
    {
        MA_TRACE("{0}", event);
    }
};

class Sandbox : public Mana::Application
{
public:
    Sandbox()
    {
        pushLayer(new ExampleLayer());
        pushOverlay(new Mana::ImGuiLayer());
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