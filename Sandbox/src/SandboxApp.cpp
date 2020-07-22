#include "Mana.h"

class Sandbox : public Mana::Application
{
public:
    Sandbox()
    {

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